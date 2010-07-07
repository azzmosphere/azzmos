
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
	apath   TEXT,
	auri    INTEGER
) RETURNS INTEGER AS $$
DECLARE 
	fqpid    INTEGER;
	schemeid INTEGER;
	authid   INTEGER;
BEGIN
	schemeid := NULL;
	SELECT id INTO schemeid FROM scheme
		WHERE value = ascheme;


	IF schemeid IS NULL THEN
		SELECT nextval('seq_scheme') INTO schemeid;	
		INSERT INTO scheme( id, value) VALUES(
			schemeid,
			ascheme
		);
	END IF;

	authid := NULL;
	SELECT id INTO authid FROM authority 
		WHERE cname = aauth;
	IF authid IS NULL THEN
		SELECT nextval('seq_authority') INTO authid;
		INSERT INTO authority( id, cname ) VALUES(
			authid,
			aauth
		);
	END IF;
	
	fqpid := NULL;
	SELECT id INTO fqpid FROM fqp 
		WHERE scheme    = schemeid AND
		      authority = authid   AND
                      path      = apath;
	IF fqpid IS NULL THEN
		SELECT nextval('seq_fqp') INTO fqpid;
		INSERT INTO fqp( 
			id,
			scheme,
			authority,
			uri,
			path
		) 
		VALUES(
			fqpid,
			schemeid,
			authid,
			auri,
			apath
		);
		
	END IF;
	RETURN fqpid;
		
END;
$$ LANGUAGE plpgsql;
	
