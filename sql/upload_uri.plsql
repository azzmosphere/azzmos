
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  upload_uri
 *  Description:  upload a URI to the database and return
 *                the uriid.
 *                if the URI exists the update it and return the uri id
 *                otherwise insert it and return the uriid.
 * =====================================================================================
 */

CREATE OR REPLACE FUNCTION upload_uri(
	scheme VARCHAR(10),
	auth   TEXT,
        path   TEXT
) RETURNS INTEGER AS $$
DECLARE 
	uriid INTEGER;
BEGIN
END;
$$ LANGUAGE plpgsql;
