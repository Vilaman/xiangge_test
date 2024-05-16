drop table if exists UserInfo;
drop table if exists Userlog;
create table UserInfo(
user_id integer primary key autoincrement,
user_account not null unique,
user_password text not null,
user_name text not null,
user_age integer not null default 0,
user_sex text not null default "",
user_address text not null default "",
user_isLogin integer not null default 0,
user_isAdmin integer not null default 0
);

create table Userlog(
op_id integer primary key autoincrement,
op_account text not null,
op_type text not null,
op_content text not null,
op_ip text not null,
op_port integer not null,
op_time text not null
);

