SELECT
    bl.pid AS blocked_pid,
    ka.query AS blocking_statement,
    now() - ka.query_start AS blocking_duration,
    ba.query AS blocked_statement,
    now() - ba.query_start AS blocked_duration
FROM pg_catalog.pg_locks bl
JOIN pg_catalog.pg_stat_activity ba ON bl.pid = ba.pid
JOIN pg_catalog.pg_locks kl ON bl.locktype = kl.locktype 
    AND bl.relation IS NOT DISTINCT FROM kl.relation
    AND bl.tuple IS NOT DISTINCT FROM kl.tuple
    AND bl.transactionid IS NOT DISTINCT FROM kl.transactionid
    AND bl.pid != kl.pid
JOIN pg_catalog.pg_stat_activity ka ON kl.pid = ka.pid
WHERE NOT bl.granted;
