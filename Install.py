import sqlite3
from pathlib import Path

DATA_DIR = Path('Data')
DB_NAME = DATA_DIR / 'SomeDatabase.db'

def PrintProgramName(phrase: str = ""):
    programName = '[Install.py] '

    finalPhrase = programName + phrase
    print(finalPhrase)

def CreateTables(cursor):
    print('[CREATING TABLES]')

    print('TABLE: Users')
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS Users
        (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Email TEXT NOT NULL
        );
    """)
    print()

def DeleteData(cursor):
    print('[DELETING DATA]')

    print('TABLE: Users')
    cursor.execute("""
        DELETE FROM Users;
    """)
    print()

def InsertData(cursor):
    print('[INSERTING DATA]')

    print('TABLE: Users')
    cursor.execute("""
        INSERT INTO Users (Name, Email)
        VALUES ('Alex', 'letmeexist95@gmail.com');
    """)
    cursor.execute("""
        INSERT INTO Users (Name, Email)
        VALUES ('Someone', 'SomeEmail@gmail.com');
    """)
    cursor.execute("""
        INSERT INTO Users (Name, Email)
        VALUES ('EndRow', 'EndRowEmail@gmail.com');
    """)
    print()


if __name__ == '__main__':
    PrintProgramName();

    if not DATA_DIR.exists():
        print('Creating data directory')
        DATA_DIR.mkdir()

    connection = sqlite3.connect(DB_NAME)
    cursor = connection.cursor()

    print('[DATABASE INIT START]\n')

    CreateTables(cursor)
    DeleteData(cursor)
    InsertData(cursor)

    connection.commit()
    connection.close()

    print('\n[DATABASE INIT FINISH]')
    PrintProgramName()
