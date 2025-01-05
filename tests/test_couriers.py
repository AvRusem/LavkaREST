import pytest

from testsuite.databases import pgsql


@pytest.mark.xfail(reason="Feature not implemented yet")
@pytest.mark.pgsql('couriers')
async def test_create_couriers_success(service_client):
    '''
    Test successfully create couriers
    '''

    payload = {
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


@pytest.mark.xfail(reason="Feature not implemented yet")
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
                "courier_id": 1,
                "courier_type": "DRONE",
                "regions": [1],
                "working_hours": ["09:00-18:00"]
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400
    assert response.json() == {"message": "invalid courier_type: DRONE"}

    payload = {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [],
                "working_hours": ["09:00-18:00"]
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400
    assert response.json() == {"message": "regions cannot be empty"}

    payload = {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [1, 2],
                "working_hours": []
            }
        ]
    }

    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400
    assert response.json() == {"message": "working_hours cannot be empty"}

    cursor = pgsql['couriers'].cursor()
    cursor.execute("SELECT COUNT(*) FROM couriers.couriers")
    result = cursor.fetchone()
    assert result[0] == 0


@pytest.mark.xfail(reason="Feature not implemented yet")
@pytest.mark.pgsql('couriers', files=['couriers_initial_data.sql'])
async def test_create_couriers_unique_id(service_client):
    '''
    Test got error creating a courier with not unique id:
    ID must be unique, on BadRequest nothing changes
    '''

    cursor = pgsql['couriers'].cursor()
    cursor.execute("SELECT COUNT(*) FROM couriers.couriers")
    result = cursor.fetchone()
    start_num = result[0]

    payload = {
        "couriers": [
            {
                "courier_id": 1,
                "courier_type": "FOOT",
                "regions": [1],
                "working_hours": ["09:00-18:00"]
            },
            {
                "courier_id": 10,
                "courier_type": "BIKE",
                "regions": [1],
                "working_hours": ["09:00-18:00"]
            }
        ]
    }
    response = await service_client.post('/couriers', json=payload)
    assert response.status == 400
    assert response.json() == {"message": "courier_id must be unique"}

    cursor.execute("SELECT COUNT(*) FROM couriers.couriers")
    result = cursor.fetchone()
    assert result[0] == start_num


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
