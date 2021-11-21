#include <stdio.h>
#include <sqlite3.h>
#include <string>

using namespace std;

class Database {
private:
    sqlite3 *db;
    char *ErrMsg = 0;
    inline static int resultInt = 0;
    inline static bool resultBool = false;
    const char *data = "Callback function called";

    static int Callback(void *data, int argc, char **argv, char **ColName) {
        resultBool = false;
        string colNameString = *ColName;

        if (colNameString == "water") {
            string resultString = argv[0];
            int water = stoi(resultString);
            resultInt += water;
        } else if (colNameString == "id") {
            resultBool = true;
        } else {
            resultInt = 0;
            string resultString = argv[0];
            resultInt = stoi(resultString);
        }
        return 0;
    }

public:
    void OpenDatabase(const char *name) {
        int rc = sqlite3_open(name, &db);

        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }
    }
    void CloseDatabase() {
        sqlite3_close(db);
        fprintf(stdout, "Closed database successfully\n");
    }
    bool CheckIfUserExist(int id) {
        resultBool = false;
        string stringSql = "SELECT id FROM Users WHERE id=" + to_string(id);
        int rc = sqlite3_exec(db, stringSql.c_str(), Callback, 0, &ErrMsg);
        return resultBool;
    }
    void AddUser(int id, int age, int wage) {
        if (!CheckIfUserExist(id)) {
        string stringSql =
                "INSERT OR IGNORE INTO Users (id, age, wage) VALUES (\"" + to_string(id) + "\", \"" + to_string(age) +
                "\", \"" + to_string(wage) + "\");";
        int rc = sqlite3_exec(db, stringSql.c_str(), Callback, 0, &ErrMsg);
        }
    }
    void AddUserRecord(int id, int water) {
        string stringSql =
                "INSERT INTO records (id, water) VALUES (\"" + to_string(id) + "\", \"" + to_string(water) + "\");";
        char rc = sqlite3_exec(db, stringSql.c_str(), Callback, 0, &ErrMsg);
    }
    int GetUserAge(int id) {
        string stringSql = "SELECT age FROM Users WHERE id=" + to_string(id);
        int rc = sqlite3_exec(db, stringSql.c_str(), Callback, 0, &ErrMsg);
        return resultInt;
    }
    int GetUserWage(int id) {
        string stringSql = "SELECT wage FROM Users WHERE id=" + to_string(id);
        int rc = sqlite3_exec(db, stringSql.c_str(), Callback, 0, &ErrMsg);
        return resultInt;
    }
    int GetUserWater(int id, string date) {
        string stringSql = "SELECT water FROM records WHERE id=" + to_string(id) + " AND dateWater='" + date + "'";
        int rc = sqlite3_exec(db, stringSql.c_str(), Callback, 0, &ErrMsg);
        return resultInt;
    }
};

int main(int argc, char *argv[]) {
    Database database;
    database.OpenDatabase("DatabaseTelegramBot.db");
    database.AddUser(13333, 12, 50);
    database.CloseDatabase();

    return 0;
}
