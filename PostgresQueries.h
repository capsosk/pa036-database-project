//
// Created by capsosk on 11/05/2021.
//

#pragma once

#include <string>

constexpr auto pg_address = "postgresql://testuser:qwerty@localhost/testdb";

enum class PostgresType
{
    JSON,
    JSONB,
    SCHEMA
};

//! inserts
static std::string queryJson;
static std::string queryJsonb;

//! indexes
const std::string index_basic_jsonb = R"(CREATE INDEX idx_person_jsonb_birthdate
                                    ON person_jsonb((data ->> 'birthdate'));)";
const std::string index_extra_jsonb = R"(CREATE INDEX idx_person_jsonb_job ON person_jsonb((data -> 'job' ->> 'salary'));)";
const std::string index_basic = R"(
CREATE INDEX idx_person_json_birthdate ON person_json((data ->> 'birthdate'));)";
const std::string index_extra = R"(
CREATE INDEX idx_person_json_job ON person_json((data -> 'job' ->> 'salary'));)";
const std::string index_schema = R"(CREATE INDEX idx_person_schema_birthdate ON person_schema(birthdate);)";
const std::string index_schema2 = R"(CREATE INDEX idx_job ON job(salary);)";

//! Create table
const std::string create_table_jsonb = R"(CREATE TABLE person_jsonb (data jsonb))";
const std::string create_table_json = R"(CREATE TABLE person_json (data json);)";
const std::string create_table_schema_person = R"(CREATE TABLE person_schema (
	name VARCHAR(255),
	surname VARCHAR(255),
	birthdate date,
	address_id INT,
	job_id INT,
	phone_numbers VARCHAR(10)[3]);
)";
const std::string create_table_schema_address = R"(CREATE TABLE address (
	id SERIAL,
	city VARCHAR(255),
	street VARCHAR(255),
	street_number INT,
	postal_code INT);
)";
const std::string create_table_schema_job = R"(CREATE TABLE job (
	id SERIAL,
	company_name VARCHAR(255),
	salary INT,
	start_date date);
)";

//! create insert string for Schema
inline std::string createInsertSchema(const Person &person)
{
    std::stringstream retval;
    Address address = person.address;
    Job job = person.job;
    auto numbers = person.phoneNumbers;
    retval << "WITH insert_address AS (INSERT INTO address(city, street, street_number, postal_code) VALUES ("
           << "\'" << address.city << "\'"
           << ","
           << "\'" << address.street << "\'"
           << ","
           << address.streetNumber << ","
           << "\'" << address.postalCode << "\'"
           << ") RETURNING id AS inserted_address_id), insert_job AS (INSERT INTO job (company_name, salary, start_date) VALUES ("
           << "\'" << job.companyName << "\'"
           << ","
           << job.salary << ","
           << "\'" << job.startDate << "\'"
           << ") RETURNING id AS inserted_job_id) INSERT INTO person_schema VALUES ("
           << "\'" << person.name << "\'"
           << ","
           << "\'" << person.surname << "\'"
           << ","
           << "\'" << person.birthdate << "\'"
           << ","
           << "(select inserted_address_id from insert_address), (select inserted_job_id from insert_job),"

           << "\'{";
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        retval << "\"" << *it << "\"";
        if (it + 1 != numbers.end()) {
            retval << ",";
        }
    }
    retval << "}\')";

    return retval.str();
}

inline std::string createInsertJson(const std::vector<std::string> &objects)
{
    std::string begin = "INSERT INTO person_json VALUES";
    std::string core;
    for (auto &item : objects) {
        core.append("(\'" + item + "\'),");
    }
    core.pop_back();
    core.append(";");
    return begin + core;
}

inline std::string createInsertJsonb(const std::vector<std::string> &objects)
{
    std::string begin = "INSERT INTO person_jsonb VALUES";
    std::string core;
    for (auto &item : objects) {
        core.append("(\'" + item + "\'::jsonb),");
    }
    core.pop_back();
    core.append(";");
    return begin + core;
}

//! select outer
const std::string selectOuterJsonb = R"(SELECT data -> 'name' FROM public.person_jsonb WHERE (data ->> 'birthdate') >= '1970-01-01' AND (data ->> 'birthdate') < '1980-01-01';)";
const std::string selectOuterJson = R"(SELECT data -> 'name' FROM public.person_json WHERE (data ->> 'birthdate') >= '1970-01-01' AND (data ->> 'birthdate') < '1980-01-01';)";
const std::string selectOuterSchema = R"(
SELECT name
FROM public.person_schema
WHERE birthdate >= '1970-01-01' AND birthdate < '1980-01-01';
)";

//! select inner
const std::string selectInnerJsonb = R"(SELECT data -> 'name' FROM public.person_jsonb WHERE (data -> 'job' ->> 'salary')::INT < 1000;)";
const std::string selectInnerJson = R"(SELECT data -> 'name' FROM public.person_json WHERE (data -> 'job' ->> 'salary')::INT < 1000;)";
const std::string selectInnerSchema = R"(
SELECT job.salary
FROM person_schema JOIN job ON person_schema.job_id = job.id WHERE job.salary < 1000;
)";

//! aggregate
const std::string aggregateJsonb = R"(
SELECT extract(year from (person_jsonb.data ->> 'birthdate')::DATE) as extracted_year, AVG((data -> 'job' ->> 'salary')::INT)
FROM public.person_jsonb
WHERE (data -> 'job' ->> 'salary')::INT < 1000
GROUP BY extracted_year;
)";
const std::string aggregateJson = R"(
SELECT extract(year from (person_json.data ->> 'birthdate')::DATE) as extracted_year, AVG((data -> 'job' ->> 'salary')::INT)
FROM public.person_json
WHERE (data -> 'job' ->> 'salary')::INT < 1000
GROUP BY extracted_year;
)";
const std::string aggregateSchema = R"(
SELECT extract(year from person_schema.birthdate) as extracted_year, AVG(job.salary)
FROM person_schema JOIN job ON person_schema.job_id = job.id
WHERE job.salary < 1000
GROUP BY extracted_year;)";

//! update
const std::string updateJsonb = R"(
UPDATE public.person_jsonb
SET data = jsonb_set(data, '{job,salary}', jsonb '3000')
WHERE (data ->> 'birthdate') < '1970-01-01';
)";
const std::string updateJson = R"(
UPDATE public.person_json
SET data = jsonb_set(data::jsonb, '{job,salary}', jsonb '3000')
WHERE (data ->> 'birthdate') < '1970-01-01';
)";
const std::string updateSchema = R"(
UPDATE job SET salary = 3000
WHERE id IN (SELECT job_id FROM person_schema WHERE birthdate < '1970-01-01');
)";
