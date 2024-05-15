drop table if exists UserInfo;
drop table if exists Userlog;
create table UserInfo(
user_id integer primary key autoincrement,
user_name text not null,
user_password text not null,
user_sex text not null,
user_age integer not null,
user_position text not null,
user_salary real not null,
user_phone text not null unique
);

create table Userlog(
user_ip text not null,
user_port int not null,
user_status text not null,
user_operation_time text not null
);

