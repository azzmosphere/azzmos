/**
 * freq_bucket,
 *
 * Keeps a time span of how frequently a URI is
 * updated.
 */

CREATE TABLE freq_bucket (
	id      INTEGER   PRIMARY KEY,
	range   INTERVAL  NOT NULL
);

CREATE SEQUENCE seq_freq_bucket;

/* Initial infinate timespan (by infinate this means 100 years)
 * ALl URIs get placed into this buket when they are first 
 * created and then moved by azzmos as they are checked.
 */
INSERT INTO freq_bucket(id, range) 
VALUES(
	nextval('seq_freq_bucket'),
	'36500 00:00:00'
);
