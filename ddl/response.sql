CREATE TABLE response(
	id     INTEGER PRIMARY KEY,
	value  TEXT    NOT NULL,
	uri    INTEGER NOT NULL,
	FOREIGN KEY(uri)    REFERENCES uri(id)
);
CREATE SEQUENCE seq_response;
