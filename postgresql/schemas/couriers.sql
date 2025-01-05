DROP SCHEMA IF EXISTS couriers CASCADE;

CREATE SCHEMA IF NOT EXISTS couriers;

CREATE TYPE couriers.delivery AS enum (
    'FOOT', 'BIKE', 'AUTO'
);

CREATE TABLE IF NOT EXISTS couriers.couriers (
    courier_id SERIAL PRIMARY KEY,
    courier_type delivery NOT NULL,
    regions INTEGER[] NOT NULL,     
    working_hours TEXT[] NOT NULL
);
