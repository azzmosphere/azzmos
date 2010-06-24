CREATE TABLE  headers (
	htypeid  INTEGER NOT NULL,
	hdrgrpid INTEGER NOT NULL,
	cdate    TIMESTAMP NOT NULL DEFAULT now(),
	value    TEXT NOT NULL,
	PRIMARY KEY( htypeid, hdrgrpid),
	FOREIGN KEY(htypeid) REFERENCES header_types(id)
);
CREATE SEQUENCE seq_hdrgrpid;

