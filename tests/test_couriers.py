import pytest

from testsuite.databases import pgsql


@pytest.mark.xfail(reason="POST /couriers not implemented yet")
@pytest.mark.pgsql('couriers')
async def test_create_couriers_success(service_client):
    '''
    Test successfully create couriers
    '''

    payload = {
        "couriers": [
            {
                "courier_type": "FOOT",
                "regions": [1, 2, 3],
                "working_hours": ["09:00-18:00"]
            },
            {
                "courier_type": "BIKE",
                "regions": [4, 5],
                "working_hours": ["10:00-20:00"]
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [1, 2, 3],
                "working_hours": ["09:00-18:00"]
            },
            {
                "courier_id": 2,
                "courier_type": "BIKE",
                "regions": [4, 5],
                "working_hours": ["10:00-20:00"]
            }
        ]
    }

    cursor = pgsql['couriers'].cursor()
    cursor.execute("SELECT * FROM couriers.couriers")
    result = cursor.fetchall()
    assert result == [
        (1, 'FOOT', [1, 2, 3], ['09:00-18:00']),
        (2, 'BIKE', [4, 5], ['10:00-20:00'])
    ]


@pytest.mark.xfail(reason="POST /couriers not implemented yet")
async def test_create_couriers_error(service_client):
    '''
    Test with errors:
    Client can choose only existing delivery types;
    Regions cannot be empty;
    Working hours cannot be empty;
    Courier's ID must be unique
    '''

    payload = {
        "couriers": [
            {
                "courier_type": "DRONE",
                "regions": [1],
                "working_hours": ["09:00-18:00"]
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400

    payload = {
        "couriers": [
            {
                "courier_type": "FOOT",
                "regions": [],
                "working_hours": ["09:00-18:00"]
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400

    payload = {
        "couriers": [
            {
                "courier_type": "FOOT",
                "regions": [1, 2],
                "working_hours": []
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400

    cursor = pgsql['couriers'].cursor()
    cursor.execute("SELECT COUNT(*) FROM couriers.couriers")
    result = cursor.fetchone()
    assert result[0] == 0


@pytest.mark.xfail(reason="GET /couriers/{courier_id} not implemented yet")
@pytest.mark.pgsql('couriers', files=['couriers_initial_data.sql'])
async def test_get_courier_id_success(service_client):
    '''
    Test successfully fetching courier by id
    '''

    courier_id = 1
    response = await service_client.get('/couriers/{courier_id}')
    assert response.status == 200
    assert response.json() == {
        "courier_id": 1,
        "courier_type": "FOOT",
        "regions": [1, 2, 3],
        "working_hours": ["09:00-18:00"]
    }


@pytest.mark.xfail(reason="GET /couriers/{courier_id} not implemented yet")
@pytest.mark.pgsql('couriers', files=['couriers_initial_data.sql'])
async def test_get_courier_id_error(service_client):
    '''
    Test with errors:
    Bad request(400) when courier_id is invalid;
    Not Found(404) when courier with courier_id doesn't exist
    '''

    courier_id = 'abc'
    response = await service_client.get(f'/couriers/{courier_id}')
    assert response.status == 400

    courier_id = ''
    response = await service_client.get(f'/couriers/{courier_id}')
    assert response.status == 400

    courier_id = 42
    response = await service_client.get(f'/couriers/{courier_id}')
    assert response.status == 404


@pytest.mark.xfail(reason="GET /couriers not implemented yet")
@pytest.mark.pgsql('couriers', files=['couriers_initial_data.sql'])
async def test_get_couriers_success(service_client):
    '''
    Test successfully fetching couriers:
    if limit > len(couriers) API will return len(couriers),
        couriers after offset can be empty
    '''

    response = await service_client.get('/couriers')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [1, 2, 3],
                "working_hours": ["09:00-18:00"]
            }
        ],
        "limit": 1,
        "offset": 0
    }

    response = await service_client.get('/couriers?offset=1')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 2,
                "courier_type": "BIKE",
                "regions": [4, 5],
                "working_hours": ["10:00-20:00"]
            }
        ],
        "limit": 1,
        "offset": 1
    }

    response = await service_client.get('/couriers?limit=2')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [1, 2, 3],
                "working_hours": ["09:00-18:00"]
            },
            {
                "courier_id": 2,
                "courier_type": "BIKE",
                "regions": [4, 5],
                "working_hours": ["10:00-20:00"]
            }
        ],
        "limit": 2,
        "offset": 0
    }

    response = await service_client.get('/couriers?limit=3')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [1, 2, 3],
                "working_hours": ["09:00-18:00"]
            },
            {
                "courier_id": 2,
                "courier_type": "BIKE",
                "regions": [4, 5],
                "working_hours": ["10:00-20:00"]
            }
        ],
        "limit": 3,
        "offset": 0
    }

    response = await service_client.get('/couriers?limit=2&offset=1')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 2,
                "courier_type": "BIKE",
                "regions": [4, 5],
                "working_hours": ["10:00-20:00"]
            }
        ],
        "limit": 2,
        "offset": 1
    }

    response = await service_client.get('/couriers?limit=2&offset=2')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
        ],
        "limit": 2,
        "offset": 2
    }


@pytest.mark.xfail(reason="GET /couriers not implemented yet")
@pytest.mark.pgsql('couriers', files=['couriers_initial_data.sql'])
async def test_get_couriers_error(service_client):
    '''
    Test with errors:
    Limit and offset must be numbers;
    Limit and offset must be non-negative
    '''

    response = await service_client.get('/couriers?limit=abc')
    assert response.status == 400

    response = await service_client.get('/couriers?offset=-1')
    assert response.status == 400

    response = await service_client.get('/couriers?limit=-1offset=a')
    assert response.status == 400

    response = await service_client.get('/couriers?limit=offset=')
    assert response.status == 400

    response = await service_client.get('/couriers?offset=')
    assert response.status == 400

    response = await service_client.get('/couriers?limit=[1, 2]')
    assert response.status == 400
