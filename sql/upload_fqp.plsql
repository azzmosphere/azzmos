
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  upload_fqp
 *  Description:  Return the FQP id if it does not all ready
 *                exist or create a new FQP and return the 
 *                id.
 * =====================================================================================
 */

CREATE OR REPLACE FUNCTION upload_fqp (
	ascheme VARCHAR(10),
	aauth   TEXT,
	apath   TEXT
) RETURNS INTGER AS $$
DECLARE 
	fqpid    INTEGER;
	schemeid INTEGER;
	authid   INTEGER;
	pathid   INTEGER;
BEGIN
	schemeid := 0;
	SELECT id INTO schemeid FROM scheme
		WHERE value = ascheme;

	IF schemeid = 0 THEN
		SELECT nextval('seq_scheme') INTO schemeid;	
		INSERT INTO scheme( id, value) VALUES(
			schemeid,
			ascheme
		);
	END IF;

	authid := 0;
	SELECT id INTO authid FROM authority 
		WHERE cname = aauth;
	IF authid = 0 THEN
		SELECT nextval('seq_authority') INTO authid;
		INSERT INTO authority( id, cname ) VALUES(
			authid,
			aauth
		);
	END IF;
		
END;
$$ LANGUAGE plpgsql;
	
