CREATE TABLE authority (
	id      INTEGER PRIMARY KEY,
	cname   TEXT NOT NULL UNIQUE
);
CREATE SEQUENCE seq_authority;
