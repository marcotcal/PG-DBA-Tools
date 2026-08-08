SELECT 
    c.oid::regclass AS table_name,
    age(c.relfrozenxid) AS xid_age,
    mxid_age(c.relminmxid) AS mxid_age,
    ROUND(
        (100.0 * age(c.relfrozenxid) / current_setting('autovacuum_freeze_max_age')::numeric), 
        2
    ) AS freeze_risk_percent
FROM pg_class c
JOIN pg_namespace n ON n.oid = c.relnamespace
WHERE c.relkind IN ('r', 't') 
ORDER BY xid_age DESC
LIMIT 10;
