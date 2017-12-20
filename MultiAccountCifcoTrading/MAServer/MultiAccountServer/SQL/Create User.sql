create tablespace ts_zqdb datafile 'ts_zqdb.dbf' size 100M autoextend on next 10M maxsize 1000M;

create temporary tablespace ts_tempdb tempfile 'ts_tmpdb.dbf' size 32M autoextend on next 32M maxsize 2048M extent management local;

create user maaccount identified by maaccount 
       default tablespace ts_zqdb
       temporary tablespace ts_tempdb;
       
grant connect, resource to maaccount;
