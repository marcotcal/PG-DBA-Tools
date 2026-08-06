SELECT 
    schemaname,
    relname,
    last_autovacuum,
    last_vacuum
FROM pg_stat_user_tables
ORDER BY last_autovacuum ASC NULLS FIRST;

