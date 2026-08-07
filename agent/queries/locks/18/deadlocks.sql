SELECT 
    datname AS database_name,
    deadlocks AS total_deadlocks
FROM pg_stat_database
WHERE deadlocks > 0
ORDER BY deadlocks DES