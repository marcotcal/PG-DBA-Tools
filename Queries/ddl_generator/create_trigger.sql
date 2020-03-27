WITH
    triggers as (
        SELECT 
            pg_get_triggerdef(trg.oid) AS create_trigger
        FROM 
            pg_trigger trg
            INNER JOIN pg_class tbl ON trg.tgrelid = tbl.oid
            INNER JOIN pg_namespace n ON tbl.relnamespace = n.oid
        WHERE        
            tbl.relname !~ '^pg_'
            AND tbl.relname  =  'clinic' 
            AND n.nspname = 'clinics'
            --AND tgisinternal = FALSE
    )   
    SELECT  
        replace(
            replace(
                replace(
                    replace(
                        replace(
                            replace(
                                replace(
                                    replace(
                                        replace(create_trigger, 'BEFORE ', E'\n    BEFORE '
                                        ), 'AFTER', E'\n    AFTER'
                                    ), 'ON ', E'\n    ON '       
                                ), 'FROM', E'\n    FROM'
                            ), 'NOT DEFERRABLE', E'\n    NOT DEFERRABLE'
                        ), 'INITIALLY', E'\n    INITIALLY'  
                    ), 'FOR ', E'\n    FOR '
                ), 'EXECUTE', E'\n    EXECUTE'
            ), 'WHEN ', E'\n    WHEN '  
        )    
    FROM triggers t