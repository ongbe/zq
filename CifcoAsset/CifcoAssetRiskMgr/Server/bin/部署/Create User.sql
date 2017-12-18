create tablespace ts_riskdb datafile 'ts_riskdb.dbf' size 100M autoextend on next 10M maxsize 1000M;

create temporary tablespace ts_tempriskdb tempfile 'ts_tempriskdb.dbf' size 32M autoextend on next 32M maxsize 2048M extent management local;

create user risk identified by risk 
       default tablespace ts_riskdb
       temporary tablespace ts_tempriskdb;
       
grant connect, resource to risk;
