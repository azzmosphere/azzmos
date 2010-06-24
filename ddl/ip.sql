CREATE TABLE ip(
	id        INTEGER PRIMARY KEY,
	address   TEXT UNIQUE NOT NULL,
	ipv4      BOOLEAN,
	authority INTEGER,
	FOREIGN KEY(authority) REFERENCES authority(id)
);
CREATE SEQUENCE seq_ip;
