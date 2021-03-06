PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE questions (id integer primary key asc, text);
INSERT INTO questions VALUES(1,'Do you identify as a human?');
INSERT INTO questions VALUES(2,'Are you cisgender?');
INSERT INTO questions VALUES(3,'Can you drive a car?');
INSERT INTO questions VALUES(4,'Do you primarily use public transportation?');
INSERT INTO questions VALUES(5,'Have you ever played Among Us?');
INSERT INTO questions VALUES(6,'Have you seen The Owl House?');
INSERT INTO questions VALUES(7,'Have you seen Gravity Falls?');
INSERT INTO questions VALUES(8,'Do you live in the U.S.?');
INSERT INTO questions VALUES(9,'Do you often change your Tumblr URL?');
INSERT INTO questions VALUES(10,'Are you an adult?');
INSERT INTO questions VALUES(11,'Have you ever had a pet?');
INSERT INTO questions VALUES(12,'Do you use Windows primarily?');
INSERT INTO questions VALUES(13,'Is English your primary language?');
INSERT INTO questions VALUES(14,'Do you play any sport?');
INSERT INTO questions VALUES(15,'Do you use exclusively binary (he/him or she/her) pronouns?');
INSERT INTO questions VALUES(16,'Do you use neopronouns?');
INSERT INTO questions VALUES(17,'Would you consider yourself extraordinarily weird? (If you consider yourself moderately weird, choose &quot;false&quot;)');
INSERT INTO questions VALUES(18,'Have you ever been in a relationship?');
INSERT INTO questions VALUES(19,'Do you say &quot;favourite&quot; (true) or &quot;favorite&quot; (false)?');
INSERT INTO questions VALUES(20,'Do you believe that pineapple belongs on pizza?');
INSERT INTO questions VALUES(21,'Can you whistle?');
INSERT INTO questions VALUES(22,'Can you cook?');
INSERT INTO questions VALUES(23,'Do you like to dance?');
CREATE TABLE characters (name, q_1, q_2, q_3, q_4, q_5, q_6, q_7, q_8, q_9, q_10, q_11, q_12, q_13, q_14, q_15, q_16, q_17, q_18, q_19, q_20, q_21, q_22, q_23);
INSERT INTO characters VALUES('kit',0.10000000000000000555,-0.19000000000000000222,0.19000000000000000222,0.010000000000000000208,0.19000000000000000222,-0.19000000000000000222,0.19000000000000000222,0.19000000000000000222,-0.10000000000000000555,-0.19000000000000000222,0.10000000000000000555,-0.19000000000000000222,0.19000000000000000222,0.19000000000000000222,-0.19000000000000000222,-0.19000000000000000222,0.19000000000000000222,-0.10000000000000000555,-0.19000000000000000222,0.10000000000000000555,0.19000000000000000222,0.19000000000000000222,0.19000000000000000222);
CREATE TABLE keys (value, expiry);
CREATE TABLE verinfo (key, value);
INSERT INTO verinfo VALUES('ver','3.2');
INSERT INTO verinfo VALUES('db_ver','2');
INSERT INTO verinfo VALUES('last_update',1656864389);
COMMIT;
