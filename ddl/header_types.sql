CREATE TABLE header_types (
	id     INTEGER PRIMARY KEY,
	value  TEXT NOT NULL UNIQUE
);
CREATE SEQUENCE seq_header_types;
