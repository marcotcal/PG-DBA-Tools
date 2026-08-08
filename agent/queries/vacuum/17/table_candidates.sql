SELECT 
    schemaname,
    relname AS table_name,
    n_dead_tup AS dead_rows,
    n_live_tup AS live_rows,
    ROUND(100.0 * n_dead_tup / NULLIF(n_dead_tup + n_live_tup, 0),2) AS dead_rows_ratio_percent,
    last_vacuum,
    last_autovacuum
FROM pg_stat_user_tables
WHERE (n_dead_tup + n_live_tup) > 0
ORDER BY n_dead_tup DESC
LIMIT 10;
