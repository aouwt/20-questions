pragma foreign_keys=OFF;
begin transaction;

create table questions (id integer primary key asc, text);
insert into questions values (1, 'Do you identify as a human?');
insert into questions values (2, 'Are you cisgender?');
insert into questions values (3, 'Can you drive a car?');
insert into questions values (4, 'Do you primarily use public transportation?');
insert into questions values (5, 'Have you ever played Among Us?');
insert into questions values (6, 'Have you seen The Owl House?');
insert into questions values (7, 'Have you seen Gravity Falls?');
insert into questions values (8, 'Do you live in the U.S.?');
insert into questions values (9, 'Do you often change your Tumblr URL?');
insert into questions values (10, 'Are you an adult?');
insert into questions values (11, 'Have you ever had a pet?');
insert into questions values (12, 'Do you use Windows primarily?');
insert into questions values (13, 'Is English your primary language?');
insert into questions values (14, '');
insert into questions values (15, '');
insert into questions values (16, '');
insert into questions values (17, '');
insert into questions values (18, '');
insert into questions values (19, '');
insert into questions values (20, '');

create table characters (name, q_1, q_2, q_3, q_4, q_5, q_6, q_7, q_8, q_9, q_10, q_11, q_12, q_13, q_14, q_15, q_16, q_17, q_18, q_19, q_20);
insert into characters values ('kit', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

create table keys (value, expiry);


commit;

