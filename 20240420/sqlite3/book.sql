drop table if exists bookuser;
create table bookuser(
user_id integer primary key autoincrement,
user_name text not null,
user_phone text not null unique
);


insert into bookuser (user_name,user_phone) values ('zhangsan','134566655444');
insert into bookuser (user_name,user_phone) values ('lisi','13567325444');
insert into bookuser (user_name,user_phone) values ('wangwu','13788566655444');



select * from bookuser where user_id=2;
