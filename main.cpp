//
// Created by capsosk on 16/04/2021.
//

#include <iostream>

#include "headers/ChronoWrapper.h"
#include "headers/FileParser.h"
#include "headers/MongoDatabase.h"
#include "headers/PostgresqlDatabase.h"
#include "PostgresQueries.h"

enum class PostgresType
{
    JSON,
    JSONB,
    SCHEMA
};

enum class IndexType
{
    INDEX,
    INDEX_EXTRA,
    NO_INDEX
};

constexpr auto kCurrentFileSize = "10000";
constexpr IndexType indexType = IndexType::INDEX;
constexpr PostgresType postgresType = PostgresType::SCHEMA;

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
void addDBObjects(PostgresqlDatabase &postgresql, T objects)
{
    switch (postgresType) {
    case PostgresType::JSON:
        postgresql.AddMultipleObjectsJson(objects);
        break;
    case PostgresType::JSONB:
        postgresql.AddMultipleObjectsJsonb(objects);
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

    auto postgresql = PostgresqlDatabase(pg_address);
    std::cout << postgresql.GetName() << " initialized" << '\n';
    postgresql.ClearDatabase();

    int i;

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

    //! insert benchmark
    std::cout << "\nInsert benchmark\n";
    i = 0;
    while (i != 10) {
        std::fstream out(std::string("PostgresInsert") + getFileName(), std::ios_base::app);
        i++;
        runDBSetup(postgresql);
        timer.startTimer();
        postgresType == PostgresType::SCHEMA ? addDBObjects(postgresql, objectsSchema) : addDBObjects(postgresql, objects);
        timer.endTimer();
        timer.displayResultOnStream(out);
        postgresql.ClearDatabase();
    }

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
    const std::string kFileName = std::string("../") + kCurrentFile + ".json";

    auto parser = FileParser(kFileName);
    const auto objects = parser.returnJsonObjects();
    mongo.DropDatabase();

    auto indexes = make_document(kvp("birthdate", 1), kvp("job.salary", 1));
    mongo.CreateIndexes(indexes);

    std::fstream out("MongoAggregate10000Index", std::ios_base::app);

    auto timer = ChronoWrapper();

    int i;

    //    //! add Benchmark
    //    out << "\nMongo Add with Index\n";
    //    i = 0;
    //    while (i != 10) {
    //        i++;
    //        timer.startTimer();
    //        mongo.AddMultipleObjectsScheme(objects);
    //        timer.endTimer();
    //        mongo.ClearDatabase();
    //        timer.displayResultOnStream(out);
    //    }

    //    //! select Benchmark Outer
    //    auto filter = document{} << "birthdate" << open_document
    //                             << "$gt"
    //                             << "1970-01-01T00:00:00.000Z"
    //                             << "$lte"
    //                             << "1980-01-01T00:00:00.000Z"
    //                             << close_document << finalize;
    //    i = 0;
    //    mongo.AddMultipleObjectsScheme(objects);
    //    out << "\nMongo Select Outer\n";
    //    while (i != 10) {
    //        i++;
    //        timer.startTimer();
    //        mongo.FindMany(filter);
    //        timer.endTimer();
    //        timer.displayResultOnStream(out);
    //    }

    //    //! select Benchmark Inner
    //    auto filter = make_document(kvp("job.salary", make_document(kvp("$lt", 1000))));
    //    i = 0;
    //    mongo.AddMultipleObjectsScheme(objects);
    //    out << "\nMongo Select Inner\n";
    //    while (i != 10) {
    //        i++;
    //        timer.startTimer();
    //        mongo.FindMany(filter);
    //        timer.endTimer();
    //        timer.displayResultOnStream(out);
    //    }

    //////! Aggregate Benchmark
    //    out << "\nMongo Aggregate\n";
    //    i = 0;
    //    mongo.AddMultipleObjectsScheme(objects);
    //    auto valueMatch = make_document(kvp("job.salary", make_document(kvp("$lt", 1000))));
    //    auto valueGroup = make_document(kvp("_id", make_document(kvp("birthdate", make_document(kvp("year", "$birthdate"))))),
    //                                    kvp("avg_salary", make_document(kvp("$avg", "$job.salary"))));
    //    while (i != 10) {
    //        i++;
    //        timer.startTimer();
    //        mongo.Aggregate(valueMatch, valueGroup);
    //        timer.endTimer();
    //        timer.displayResultOnStream(out);
    //    }

    //    //! Update Benchmark
    //    const auto toUpdate = make_document(kvp("birthdate", make_document(kvp("$lt", "1970-01-01T00:00:00.000Z"))));
    //    const auto setTo = make_document(kvp("$set", make_document(kvp("job.salary", "3000"))));
    //    i = 0;
    //    mongo.AddMultipleObjectsScheme(objects);
    //
    //    out << "\nMongo Update\n";
    //    while (i != 1) {
    //        i++;
    //        //! update is problematic, it sigsegvs at the end, probably a mongocxx bug
    //        //! we have to time it inside the function
    //        mongo.UpdateMany(toUpdate, setTo);
    //
    //    }
}

int main()
{
    //MongoBenchmark();
    PostgressBenchmark();
    return 0;
}
