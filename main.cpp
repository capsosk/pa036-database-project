//
// Created by capsosk on 16/04/2021.
//

#include <iostream>

#include "headers/ChronoWrapper.h"
#include "headers/FileParser.h"
#include "headers/MongoDatabase.h"
#include "headers/PostgresqlDatabase.h"
#include "PostgresQueries.h"

enum class IndexType
{
    INDEX,
    INDEX_EXTRA,
    NO_INDEX
};

//! PostgreSQL benchmark setup
//! we were using files with names "10000.json" and "1000000.json"
constexpr auto kCurrentFileSize = "10000";
constexpr IndexType indexType = IndexType::INDEX_EXTRA;
constexpr PostgresType postgresType = PostgresType::JSON;

void runDBSetup(PostgresqlDatabase &postgresql)
{
    switch (postgresType) {
    case PostgresType::JSON:
        postgresql.RunQuery(create_table_json);
        break;
    case PostgresType::JSONB:
        postgresql.RunQuery(create_table_jsonb);
        break;
    case PostgresType::SCHEMA:
        postgresql.RunQuery(create_table_schema_person);
        postgresql.RunQuery(create_table_schema_address);
        postgresql.RunQuery(create_table_schema_job);
        break;
    }

    if (indexType != IndexType::NO_INDEX) {
        switch (postgresType) {
        case PostgresType::JSON:
            postgresql.RunQuery(index_basic);
            if (indexType == IndexType::INDEX_EXTRA)
                postgresql.RunQuery(index_extra);
            break;
        case PostgresType::JSONB:
            postgresql.RunQuery(index_basic_jsonb);
            if (indexType == IndexType::INDEX_EXTRA)
                postgresql.RunQuery(index_extra_jsonb);
            break;
        case PostgresType::SCHEMA:
            postgresql.RunQuery(index_schema);
            if (indexType == IndexType::INDEX_EXTRA)
                postgresql.RunQuery(index_schema2);
            break;
        }
    }
}
template <typename T>
void addDBObjects(PostgresqlDatabase &postgresql, const T &objects)
{
    switch (postgresType) {
    case PostgresType::JSON:
        postgresql.AddMultipleObjectsJson_b(queryJson);
        break;
    case PostgresType::JSONB:
        postgresql.AddMultipleObjectsJson_b(queryJsonb);
        break;
    case PostgresType::SCHEMA:
        postgresql.AddMultipleObjectsScheme(objects);
        break;
    }
}

static std::string getIndexName()
{
    switch (indexType) {
    case IndexType::INDEX:
        return "indexBasic";
    case IndexType::INDEX_EXTRA:
        return "indexExtra";
    case IndexType::NO_INDEX:
        return "";
    }
}

static std::string getFileName()
{
    const std::string kIsIndex = getIndexName();
    std::string kFileSuffix = kCurrentFileSize;
    std::string kType;

    switch (postgresType) {
    case PostgresType::JSON:
        kType = "Json";
        break;
    case PostgresType::JSONB:
        kType = "JsonB";
        break;
    case PostgresType::SCHEMA:
        kType = "Schema";
        break;
    }
    return kType + kCurrentFileSize + kIsIndex;
}

void PostgressBenchmark()
{
    auto timer = ChronoWrapper();

    std::string kFileName = std::string("../") + kCurrentFileSize + ".json";
    auto parser = FileParser(kFileName);

    const auto objects = parser.returnJsonObjects();
    const auto jsonSchemeObjects = parser.returnJsonSchemeObjects();
    FileParser::jsonObjects objectsSchema;
    for (const auto &object : jsonSchemeObjects) {
        objectsSchema.push_back(createInsertSchema(object));
    }

    queryJson = createInsertJson(objects);
    queryJsonb = createInsertJsonb(objects);

    auto postgresql = PostgresqlDatabase(pg_address);
    std::cout << postgresql.GetName() << " initialized" << '\n';
    postgresql.ClearDatabase();

    int i;

    //! insert benchmark
    std::cout << "\nInsert benchmark\n";
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("PostgresInsert") + getFileName(), std::ios_base::app);
        i++;
        runDBSetup(postgresql);
        postgresType == PostgresType::SCHEMA ? addDBObjects(postgresql, objectsSchema) : addDBObjects(postgresql, objects);
        timer.displayResultOnStream(out);
        postgresql.ClearDatabase();
    }

    //! Select inner benchmark
    std::cout << "\nInner benchmark\n";
    runDBSetup(postgresql);
    postgresType == PostgresType::SCHEMA ? addDBObjects(postgresql, objectsSchema) : addDBObjects(postgresql, objects);
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("PostgresSelectInner") + getFileName(), std::ios_base::app);
        i++;
        timer.startTimer();
        switch (postgresType) {
        case (PostgresType::SCHEMA):
            postgresql.RunQuery(selectInnerSchema);
            break;
        case (PostgresType::JSON):
            postgresql.RunQuery(selectInnerJson);
            break;
        case (PostgresType::JSONB):
            postgresql.RunQuery(selectInnerJsonb);
            break;
        }
        timer.endTimer();
        timer.displayResultOnStream(out);
    }
    postgresql.ClearDatabase();

    //! Select Aggregate benchmark
    std::cout << "\nAggregate benchmark\n";
    runDBSetup(postgresql);
    postgresType == PostgresType::SCHEMA ? addDBObjects(postgresql, objectsSchema) : addDBObjects(postgresql, objects);
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("PostgresAggregate") + getFileName(), std::ios_base::app);
        i++;
        timer.startTimer();
        switch (postgresType) {
        case (PostgresType::SCHEMA):
            postgresql.RunQuery(aggregateSchema);
            break;
        case (PostgresType::JSON):
            postgresql.RunQuery(aggregateJson);
            break;
        case (PostgresType::JSONB):
            postgresql.RunQuery(aggregateJsonb);
            break;
        }
        timer.endTimer();
        timer.displayResultOnStream(out);
    }
    postgresql.ClearDatabase();

    //! Update benchmark
    std::cout << "\nUpdate benchmark\n";
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("PostgresUpdate") + getFileName(), std::ios_base::app);
        i++;
        postgresql.ClearDatabase();
        runDBSetup(postgresql);
        postgresType == PostgresType::SCHEMA ? addDBObjects(postgresql, objectsSchema) : addDBObjects(postgresql, objects);
        timer.startTimer();
        switch (postgresType) {
        case (PostgresType::SCHEMA):
            postgresql.RunQuery(updateSchema);
            break;
        case (PostgresType::JSON):
            postgresql.RunQuery(updateJson);
            break;
        case (PostgresType::JSONB):
            postgresql.RunQuery(updateJsonb);
            break;
        }
        timer.endTimer();
        timer.displayResultOnStream(out);
    }
    postgresql.ClearDatabase();

    //! Select outer benchmark
    std::cout << "\nOuter benchmark\n";

    runDBSetup(postgresql);
    postgresType == PostgresType::SCHEMA ? addDBObjects(postgresql, objectsSchema) : addDBObjects(postgresql, objects);
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("PostgresSelectOuter") + getFileName(), std::ios_base::app);
        i++;
        timer.startTimer();
        switch (postgresType) {
        case (PostgresType::SCHEMA):
            postgresql.RunQuery(selectOuterSchema);
            break;
        case (PostgresType::JSON):
            postgresql.RunQuery(selectOuterJson);
            break;
        case (PostgresType::JSONB):
            postgresql.RunQuery(selectOuterJsonb);
            break;
        }
        timer.endTimer();
        timer.displayResultOnStream(out);
    }
    postgresql.ClearDatabase();
}

void MongoBenchmark()
{
    constexpr auto mongo_address = "mongodb://localhost:27017";
    auto mongo = MongoDatabase(mongo_address);

    constexpr auto kCurrentFile = "10000";
    const std::string kIndex;
    const std::string kFileName = std::string("../") + kCurrentFile + ".json";

    auto parser = FileParser(kFileName);
    const auto objects = parser.returnJsonObjects();
    mongo.DropDatabase();

    auto indexes = make_document(kvp("birthdate", 1), kvp("job.salary", 1));

    auto timer = ChronoWrapper();

    int i;

    //! Insert Benchmark
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("MongoInsert") + kCurrentFile + kIndex, std::ios_base::app);
        i++;
        timer.startTimer();
        if (!kIndex.empty())
            mongo.CreateIndexes(indexes);
        mongo.AddMultipleObjects(objects);
        timer.endTimer();
        mongo.ClearDatabase();
        timer.displayResultOnStream(out);
    }

    //! select Benchmark Outer
    auto filterOuter = document{} << "birthdate" << open_document
                                  << "$gt"
                                  << "1970-01-01T00:00:00.000Z"
                                  << "$lte"
                                  << "1980-01-01T00:00:00.000Z"
                                  << close_document << finalize;
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("MongoSelectOuter") + kCurrentFile + kIndex, std::ios_base::app);
        i++;
        if (!kIndex.empty())
            mongo.CreateIndexes(indexes);
        mongo.AddMultipleObjects(objects);
        timer.startTimer();
        mongo.FindMany(filterOuter);
        timer.endTimer();
        timer.displayResultOnStream(out);
        mongo.ClearDatabase();
    }

    //! select Benchmark Inner
    auto filterInner = make_document(kvp("job.salary", make_document(kvp("$lt", 1000))));
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("MongoSelectInner") + kCurrentFile + kIndex, std::ios_base::app);
        i++;
        if (!kIndex.empty())
            mongo.CreateIndexes(indexes);
        mongo.AddMultipleObjects(objects);
        timer.startTimer();
        mongo.FindMany(filterInner);
        timer.endTimer();
        timer.displayResultOnStream(out);
        mongo.ClearDatabase();
    }

    //! Aggregate Benchmark
    i = 0;
    auto valueMatch = make_document(kvp("job.salary", make_document(kvp("$lt", 1000))));
    auto valueGroup = make_document(kvp("_id", make_document(kvp("birthdate", make_document(kvp("year", "$birthdate"))))),
            kvp("avg_salary", make_document(kvp("$avg", "$job.salary"))));
    while (i != 10) {
        std::fstream out(std::string("MongoAggregate") + kCurrentFile + kIndex, std::ios_base::app);
        i++;
        if (!kIndex.empty())
            mongo.CreateIndexes(indexes);
        mongo.AddMultipleObjects(objects);
        timer.startTimer();
        mongo.Aggregate(valueMatch, valueGroup);
        timer.endTimer();
        timer.displayResultOnStream(out);
        mongo.ClearDatabase();
    }

    //! Update Benchmark
    const auto toUpdate = make_document(kvp("birthdate", make_document(kvp("$lt", "1970-01-01T00:00:00.000Z"))));
    const auto setTo = make_document(kvp("$set", make_document(kvp("job.salary", "3000"))));
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("MongoUpdate") + kCurrentFile + kIndex, std::ios_base::app);
        i++;
        if (!kIndex.empty())
            mongo.CreateIndexes(indexes);
        mongo.AddMultipleObjects(objects);
        timer.startTimer();
        mongo.UpdateMany(toUpdate, setTo);
        timer.endTimer();
        timer.displayResultOnStream(out);
        mongo.ClearDatabase();
    }
}

int main()
{
    //    MongoBenchmark();
    PostgressBenchmark();
    return 0;
}
