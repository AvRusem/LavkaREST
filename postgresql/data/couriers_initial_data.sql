INSERT INTO couriers.couriers(courier_type, regions, working_hours)
VALUES -- do not change it, tests/test_courier depends on it
    ('FOOT', '{1, 2, 3}', '{"09:00-18:00"}'),
    ('BIKE', '{4, 5}', '{"10:00-20:00"}');
