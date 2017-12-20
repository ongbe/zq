create table STRATEGY_INDEX
(
  indexname    VARCHAR2(25) NOT NULL,
  ntype        INTEGER,
  tradername   VARCHAR2(25),
  nprivate     INTEGER,
  parameter    VARCHAR2(2550), 
  comments     VARCHAR2(255),
  dllfile      BLOB,
  sourcefile   BLOB,
  insertdbtime DATE,
   CONSTRAINT PK_STRATEGY_INDEX PRIMARY KEY (indexname, tradername)
);
create table STRATEGY_INDEX_RELATION
(
  strategyname  VARCHAR2(25),
  strategyindex VARCHAR2(25),
  CONSTRAINT PK_STRATEGY_INDEX_RELATION PRIMARY KEY (strategyname, strategyindex)
);
create table STRATEGY_INSTANCE
(
  strategyname          VARCHAR2(25),
  tradername            VARCHAR2(25),
  instruments           VARCHAR2(255),
  runafterload          INTEGER,
  runafteroffline       INTEGER,
  orderactionbeforestop INTEGER,
  parameter             VARCHAR2(2550),
  comments              VARCHAR2(250),
  status                INTEGER,
  insertdbtime          DATE,
  starttime             INTEGER,
  timerspan             INTEGER,
  CONSTRAINT PK_STRATEGY_INSTANCE PRIMARY KEY (strategyname, tradername)	
);
create table STRATEGY_STRATEGY
(
  strategyname     VARCHAR2(25),
  strategynickname VARCHAR2(25),
  version          VARCHAR2(11),
  tradername       VARCHAR2(21),
  priviledge       INTEGER,
  parameter        VARCHAR2(2550),
  comments         VARCHAR2(250),
  use              INTEGER,
  dllfile          BLOB,
  sourcefile       BLOB,
  insertdbtime     DATE,
  CONSTRAINT PK_STRATEGY_STRATEGY PRIMARY KEY (strategyname, tradername)
);
