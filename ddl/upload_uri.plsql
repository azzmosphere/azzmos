/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  upload_uri
 *  Description:  Return the uri id from a existing URI or 
 *                create a new uri if it does not exist.
 * =====================================================================================
 */

CREATE OR REPLACE FUNCTION upload_uri (
	ascheme TEXT,
	acname  TEXT,
	apath   TEXT
)RETURNS INTEGER AS $$
DECLARE 
	luri  INTEGER
BEGIN
	SELECT u.id INTO luri
	FROM 
		uri u 
		INNER JOIN fqp f       ON u.id = f.id
		INNER JOIN scheme s    ON f.scheme = s.id
		INNER JOIN authority a ON f.authority = a.id
	WHERE
		s.value = ascheme
		AND a.cname = acname
		AND f.path = apath;

	IF luri IS NULL THEN
		SELECT nextavl('seq_uri') INTO luri;
		INSERT INTO uri(
			id,
			last_checked,
			last_updated
		)
		VALUES(
			luri,
			now(),
			now()
		);
		SELECT upload_fqp( ascheme, acname, apath, luri);
	ELSE
		UPDATE uri SET last_checked = now() WHERE id = luri;
	END IF;
	RETURN luri;
			
END;
$$ LANGUAGE plpgsql;

