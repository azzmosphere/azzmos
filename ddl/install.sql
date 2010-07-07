/**
 * This file is used to install the database.
 */

/**
 * Create tables 
 */
\i url.sql
\i freq_bucket.sql
\i uri.sql
\i edges.sql
\i scheme.sql
\i path.sql
\i authority.sql
\i ipv4.sql
\i ipv6.sql
\i header_types.sql
\i response.sql
\i response_h.sql
\i headers.sql
\i headers_h.sql
\i fqp.sql

/*
 * start installing database functions
 */
\i upload_fqp.plsql
\i upload_uri.plsql
