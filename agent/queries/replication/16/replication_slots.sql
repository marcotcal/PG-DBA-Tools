SELECT 
    slot_name, 
    plugin, -- Identify if it is logical replication (ex: pgoutput) of physical replication
    slot_type, 
    active, 
    wal_status, -- it shows if the slot is secure, extended or  in risk
    pg_wal_lsn_diff(pg_current_wal_lsn(), restart_lsn) AS retained_bytes
FROM pg_replication_slots;