DECLARE
v_r_table_name      ALL_CONS_COLUMNS.TABLE_NAME%TYPE;
v_r_column_name     ALL_CONS_COLUMNS.COLUMN_NAME%TYPE;
v_constraint_name   ALL_CONSTRAINTS.CONSTRAINT_NAME%TYPE;
v_constraint_type   ALL_CONSTRAINTS.CONSTRAINT_TYPE%TYPE;
v_r_constraint_name ALL_CONSTRAINTS.R_CONSTRAINT_NAME%TYPE;
v_r_owner           ALL_CONSTRAINTS.R_OWNER%TYPE;
v_constraints_cnt NUMBER := 0;

 CURSOR ALL_TAB_COLUMNS_CUR IS
 SELECT ATC.TABLE_NAME
       ,ATC.COLUMN_NAME
       ,ATC.DATA_TYPE
       ,ATC.DATA_LENGTH
       ,ATC.DATA_PRECISION
       ,ATC.DATA_SCALE
       ,ATC.DEFAULT_LENGTH
       ,ATC.NULLABLE
       ,ATC.COLUMN_ID
 FROM ALL_TABLES ATT, ALL_TAB_COLUMNS ATC
 WHERE ATT.OWNER = USER   -- ENTER THE OWNER NAME.
   AND ATC.OWNER = ATT.OWNER
   AND ATC.TABLE_NAME = ATT.TABLE_NAME;

 CURSOR ALL_CONSTRAINTS_CUR(p_table_name VARCHAR2, p_column_name VARCHAR2) IS
 SELECT A.CONSTRAINT_NAME, A.CONSTRAINT_TYPE, A.R_CONSTRAINT_NAME, A.R_OWNER
 FROM ALL_CONSTRAINTS A, ALL_CONS_COLUMNS B
 WHERE A.OWNER = USER    -- ENTER THE OWNER NAME.
    AND A.CONSTRAINT_TYPE IN ('P', 'R')
    AND A.TABLE_NAME = p_table_name
    AND B.CONSTRAINT_NAME = A.CONSTRAINT_NAME
    AND B.TABLE_NAME = A.TABLE_NAME
    AND B.OWNER = A.OWNER
    AND B.COLUMN_NAME = p_column_name
 ORDER BY A.CONSTRAINT_NAME;


BEGIN

     DELETE DUMP_TABLE; -- ENTER THE TABLE NAME. 
     COMMIT;

     FOR ALL_TAB_COLUMNS_REC IN ALL_TAB_COLUMNS_CUR LOOP
          v_constraints_cnt := 0;

--dbms_output.put_line('table column is ' || ALL_TAB_COLUMNS_REC.table_name || ' ' || ALL_TAB_COLUMNS_REC.column_name);

        SELECT COUNT(*)
        INTO v_constraints_cnt
        FROM ALL_CONSTRAINTS A, ALL_CONS_COLUMNS B
        WHERE A.OWNER = USER           -- ENTER THE OWNER NAME.
           AND A.CONSTRAINT_TYPE IN ('P', 'R')
           AND A.TABLE_NAME = ALL_TAB_COLUMNS_REC.table_name
           AND B.CONSTRAINT_NAME = A.CONSTRAINT_NAME
           AND B.TABLE_NAME = A.TABLE_NAME
           AND B.OWNER = A.OWNER
           AND B.COLUMN_NAME = ALL_TAB_COLUMNS_REC.column_name;

       IF ( v_constraints_cnt = 0 ) THEN         -- No Constraints Exist.

--dbms_output.put_line('Is Zero For ' || ALL_TAB_COLUMNS_REC.table_name || ' ' || ALL_TAB_COLUMNS_REC.column_name);

               INSERT INTO DUMP_TABLE   -- ENTER THE TABLE NAME.
               (
               TABLE_NAME
                    ,COLUMN_NAME
                    ,DATA_TYPE
                    ,DATA_LENGTH
                    ,DATA_PRECISION
                    ,DATA_SCALE
                    ,DEFAULT_LENGTH
                    ,NULLABLE
                    ,COLUMN_ID
               )
               VALUES
               (
               ALL_TAB_COLUMNS_REC.TABLE_NAME
              ,ALL_TAB_COLUMNS_REC.COLUMN_NAME
              ,ALL_TAB_COLUMNS_REC.DATA_TYPE
              ,ALL_TAB_COLUMNS_REC.DATA_LENGTH
              ,ALL_TAB_COLUMNS_REC.DATA_PRECISION
              ,ALL_TAB_COLUMNS_REC.DATA_SCALE
              ,ALL_TAB_COLUMNS_REC.DEFAULT_LENGTH
              ,ALL_TAB_COLUMNS_REC.NULLABLE
              ,ALL_TAB_COLUMNS_REC.COLUMN_ID
               );
          ELSE
               FOR ALL_CONSTRAINTS_REC IN
                       ALL_CONSTRAINTS_CUR(ALL_TAB_COLUMNS_REC.TABLE_NAME,
                                   ALL_TAB_COLUMNS_REC.COLUMN_NAME) LOOP
                    v_r_table_name := ''; v_r_column_name := '';

--dbms_output.put_line('Constraint For ' || ALL_TAB_COLUMNS_REC.TABLE_NAME || ' is ' || ALL_CONSTRAINTS_REC.CONSTRAINT_NAME);

                    IF ( ALL_CONSTRAINTS_REC.CONSTRAINT_TYPE = 'R' ) THEN

--dbms_output.put_line('referred Constraint For ' || ALL_CONSTRAINTS_REC.R_CONSTRAINT_NAME || ' ' || ALL_CONSTRAINTS_REC.R_OWNER);

                         BEGIN

                         SELECT DISTINCT TABLE_NAME,COLUMN_NAME
                         INTO v_r_table_name,v_r_column_name
                         FROM ALL_CONS_COLUMNS
                         WHERE CONSTRAINT_NAME = ALL_CONSTRAINTS_REC.R_CONSTRAINT_NAME
                            AND OWNER = ALL_CONSTRAINTS_REC.R_OWNER;

                         EXCEPTION
                              WHEN NO_DATA_FOUND THEN
                                   v_r_table_name := '' ;v_r_column_name := '';
                         END;
                    END IF;

                    INSERT INTO DUMP_TABLE       -- ENTER THE TABLE NAME.
                    (
                       TABLE_NAME
                         ,COLUMN_NAME
                         ,DATA_TYPE
                         ,DATA_LENGTH
                         ,DATA_PRECISION
                         ,DATA_SCALE
                         ,DEFAULT_LENGTH
                         ,NULLABLE
                         ,COLUMN_ID
                         ,CONSTRAINT_NAME
                         ,CONSTRAINT_TYPE
                         ,R_CONSTRAINT_NAME
                         ,R_TABLE_NAME
                         ,R_COLUMN_NAME
                    )
                    VALUES
                    (
                       ALL_TAB_COLUMNS_REC.TABLE_NAME
                         ,ALL_TAB_COLUMNS_REC.COLUMN_NAME
                         ,ALL_TAB_COLUMNS_REC.DATA_TYPE
                         ,ALL_TAB_COLUMNS_REC.DATA_LENGTH
                         ,ALL_TAB_COLUMNS_REC.DATA_PRECISION
                         ,ALL_TAB_COLUMNS_REC.DATA_SCALE
                         ,ALL_TAB_COLUMNS_REC.DEFAULT_LENGTH
                         ,ALL_TAB_COLUMNS_REC.NULLABLE
                         ,ALL_TAB_COLUMNS_REC.COLUMN_ID
                         ,ALL_CONSTRAINTS_REC.CONSTRAINT_NAME
                         ,ALL_CONSTRAINTS_REC.CONSTRAINT_TYPE
                         ,ALL_CONSTRAINTS_REC.R_CONSTRAINT_NAME
                         ,v_r_table_name
                         ,v_r_column_name

                    );

            END LOOP;
        END IF; -- Constraints Exist.
     END LOOP;

     COMMIT;

END;
/
