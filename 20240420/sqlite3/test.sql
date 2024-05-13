PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE users (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL,
email TEXT NOT NULL UNIQUE
);
INSERT INTO users VALUES(1,'Alice','alice@example.com');
INSERT INTO users VALUES(3,'alice6','2532523@eq.com');
CREATE TABLE books(
book_id integer primary key autoincrement,
title text not null,
author text not null,
published_year integer
);
INSERT INTO books VALUES(2,'安徒生童话','安徒生',2020);
INSERT INTO books VALUES(3,'西游记','吴承恩',2021);
CREATE TABLE bookuser(
user_id integer primary key autoincrement,
user_name text not null,
user_phone text not null unique
);
INSERT INTO bookuser VALUES(1,'zhangsan','134566655444');
INSERT INTO bookuser VALUES(2,'lisi','13567325444');
INSERT INTO bookuser VALUES(3,'wangwu','13788566655444');
DELETE FROM sqlite_sequence;
INSERT INTO sqlite_sequence VALUES('users',3);
INSERT INTO sqlite_sequence VALUES('books',3);
INSERT INTO sqlite_sequence VALUES('bookuser',3);
COMMIT;
