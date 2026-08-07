SELECT 
    pid,
    datname AS database,
    usename AS user,
    wait_event_type,
    wait_event,
    state,
    now() - query_start AS duration,
    query
FROM pg_stat_activity
WHERE state = 'active'
  AND wait_event_type IS NOT NULL
ORDER BY duration DESC;
