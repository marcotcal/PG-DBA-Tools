WITH
triggers as (
    SELECT trg.tgname AS trigger_name,
           tbl.relname AS table_name,
           p.proname AS function_name,
           CASE trg.tgtype & cast(2 as int2)
             WHEN 0 THEN 'AFTER'
             ELSE 'BEFORE'
           END AS trigger_type,
           CASE trg.tgtype & cast(28 as int2)
             WHEN 16 THEN 'UPDATE'
             WHEN  8 THEN 'DELETE'
             WHEN  4 THEN 'INSERT'
             WHEN 20 THEN 'INSERT OR UPDATE'
             WHEN 28 THEN 'INSERT OR UPDATE OR DELETE'
             WHEN 24 THEN 'UPDATE OR DELETE'
             WHEN 12 THEN 'INSERT OR DELETE'
           END AS trigger_event,
           CASE trg.tgtype & cast(1 as int2)
             WHEN 0 THEN 'FOR EACH STATEMENT'
             ELSE 'FOR EACH ROW'
           END AS action_orientation
      FROM pg_trigger trg,
           pg_class tbl,
           pg_proc p
     WHERE trg.tgrelid = tbl.oid
       AND trg.tgfoid = p.oid
       AND tbl.relname !~ '^pg_'
    )   
    SELECT 
        'CREATE TRIGGER ' || t.trigger_name || E'\n    ' || 
        t.trigger_type || ' ' || t.trigger_event || E'\n' ||
        '    ON ' || t.table_name || E'\n' || 
        '    ' || t.action_orientation || E'\n' || 
        '    EXECUTE PROCEDURE ' || t.function_name         
    FROM triggers t