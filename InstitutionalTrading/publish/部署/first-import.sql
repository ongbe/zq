------------------------------------------------------
-- Export file for user INSTITUTION                 --
-- Created by Administrator on 2013-12-11, 13:15:30 --
------------------------------------------------------

spool d.log

prompt
prompt Creating table BASE_BROKER
prompt ==========================
prompt
create table BASE_BROKER
(
  brokerid   INTEGER not null,
  brokercode VARCHAR2(31),
  company    VARCHAR2(64),
  type       INTEGER,
  account    VARCHAR2(21),
  password   VARCHAR2(21),
  deleteflag INTEGER
)
;
comment on table BASE_BROKER
  is '经纪公司信息';
comment on column BASE_BROKER.type
  is '0：交易 使用1：行情使用';
comment on column BASE_BROKER.deleteflag
  is '删除标记       0 ：没有删除 1：已经删除';
alter table BASE_BROKER
  add constraint PK_BROKER primary key (BROKERID);

prompt
prompt Creating table BASE_ORGANIZATION
prompt ================================
prompt
create table BASE_ORGANIZATION
(
  assetmgmtorgid  INTEGER not null,
  name            VARCHAR2(20),
  upperlevelorgid INTEGER,
  scalelevel      INTEGER,
  longitude       FLOAT,
  latitude        FLOAT,
  deleteflag      INTEGER
)
;
comment on table BASE_ORGANIZATION
  is '资管组织信息';
comment on column BASE_ORGANIZATION.deleteflag
  is '删除标记       0 ：没有删除 1：已经删除';
alter table BASE_ORGANIZATION
  add constraint PK_ORGANIZATION primary key (ASSETMGMTORGID);
create unique index INDEX_ORGNAME on BASE_ORGANIZATION (NAME);
create unique index PK_ASSETMGMTORGANIZATION on BASE_ORGANIZATION (ASSETMGMTORGID);

insert into BASE_ORGANIZATION (ASSETMGMTORGID, NAME, UPPERLEVELORGID, SCALELEVEL, LONGITUDE, LATITUDE, DELETEFLAG)
values (10846209, '总部', 0, 1, 116.408386, 39.904469, 0);

prompt
prompt Creating table BASE_PRIVELEGES
prompt ==============================
prompt
create table BASE_PRIVELEGES
(
  privelegeid INTEGER not null,
  name        VARCHAR2(30),
  usertypeid  INTEGER,
  parentid    INTEGER,
  nodelevel   INTEGER
)
;
create unique index INDX_PRIVELEGES on BASE_PRIVELEGES (PRIVELEGEID, USERTYPEID);
insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (1, '账户', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (2, '委托交易账户管理', -1, 1, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (3, '增加账号', -1, 2, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (4, '修改账号', -1, 2, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (5, '删除账号', -1, 2, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (6, '登录', -1, 2, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (7, '系统用户管理', -1, 1, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (8, '增加用户', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (9, '修改用户', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (10, '删除用户', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (11, '交易员启用', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (12, '交易员初始化', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (13, '交易员分配组织架构', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (14, '交易员分配理财产品', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (15, '交易员分配委托交易账号', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (16, '风控员组织设置机构', -1, 7, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (17, '交易员出入金管理', -1, 1, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (18, '出金', -1, 17, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (19, '入金', -1, 17, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (20, '管理', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (21, '组织机构管理', -1, 20, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (22, '增加组织架构', -1, 21, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (23, '修改组织架构', -1, 21, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (24, '删除组织架构', -1, 21, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (25, '组织机构分配交易员', -1, 21, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (26, '理财产品管理', -1, 20, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (27, '增加理财产品', -1, 26, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (28, '修改理财产品', -1, 26, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (29, '删除理财产品', -1, 26, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (30, '理财产品分配交易员', -1, 26, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (31, '经纪公司服务器组管理', -1, 20, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (32, '增加经纪公司', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (33, '修改经纪公司', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (34, '删除经纪公司', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (35, '增加服务器组', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (36, '修改服务器组', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (37, '删除服务器组', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (38, '增加服务器地址', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (39, '修改服务器地址', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (40, '删除服务器地址', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (41, '修改行情经纪公司', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (42, '添加行情服务器组地址', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (43, '修改行情服务器组地址', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (44, '删除行情服务器组地址', -1, 31, 3);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (45, '策略', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (46, '增加交易策略', -1, 45, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (47, '修改交易策略', -1, 45, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (48, '删除交易策略', -1, 45, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (49, '交易策略分配组织机构', -1, 45, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (50, '权限', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (51, '增加角色', -1, 50, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (52, '修改角色', -1, 50, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (53, '删除角色', -1, 50, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (54, '角色分配权限', -1, 50, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (55, '结算', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (56, '当日数据结算', -1, 55, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (57, '历史结算', -1, 55, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (58, '自动结算时间设置', -1, 55, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (59, '查询', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (60, '查询历史基金', -1, 59, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (61, '查询历史持仓', -1, 59, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (62, '查询历史持仓明细', -1, 59, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (63, '查询历史成交', -1, 59, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (64, '查询历史报单', -1, 59, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (65, '控制', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (66, '服务器重启', -1, 65, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (67, '定时重启时间设置', -1, 65, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (68, '行情服务器重启', -1, 65, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (69, '行情服务器定时重启时间设置', -1, 65, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (70, '风控', 1, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (71, '手动审核', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (72, '限制交易', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (73, '强平', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (74, '增加组织机构风控方案', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (75, '修改组织机构风控方案', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (76, '删除组织机构风控方案', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (77, '启用开关', -1, 70, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (78, '查询', 1, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (79, '历史风险事件查询', -1, 78, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (80, '历史基金净值查询', -1, 78, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (81, '报单审核记录查询', -1, 78, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (82, '强平记录查询', -1, 78, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (83, '交易员基础信息查询', -1, 78, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (84, '消息', 1, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (84, '消息', 0, -1, 1);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (85, '消息查询', -1, 84, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (86, '消息发送', -1, 84, 2);

insert into BASE_PRIVELEGES (PRIVELEGEID, NAME, USERTYPEID, PARENTID, NODELEVEL)
values (87, '立即自动结算', -1, 55, 2);

prompt
prompt Creating table BASE_PRODUCT
prompt ===========================
prompt
create table BASE_PRODUCT
(
  financeproductid INTEGER not null,
  name             VARCHAR2(50),
  description      VARCHAR2(200),
  deleteflag       INTEGER
)
;
comment on table BASE_PRODUCT
  is '理财产品数据结构详细信息表';
comment on column BASE_PRODUCT.deleteflag
  is '删除标记       0 ：没有删除 1：已经删除';
alter table BASE_PRODUCT
  add constraint PK_PRODUCTINFO primary key (FINANCEPRODUCTID);
create unique index PK_FPRODUCTINFO on BASE_PRODUCT (FINANCEPRODUCTID);

prompt
prompt Creating table BASE_ROLE
prompt ========================
prompt
create table BASE_ROLE
(
  roleid     INTEGER not null,
  name       VARCHAR2(20) not null,
  usertypeid INTEGER,
  deleteflag INTEGER
)
;
comment on column BASE_ROLE.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_ROLE
  add constraint PK_ROLE primary key (ROLEID);
create unique index PK_ROLEINFO on BASE_ROLE (ROLEID);

prompt
prompt Creating table BASE_SERVERADDR
prompt ==============================
prompt
create table BASE_SERVERADDR
(
  addrid     INTEGER not null,
  serverid   INTEGER,
  address    VARCHAR2(128),
  port       INTEGER,
  deleteflag INTEGER
)
;
comment on table BASE_SERVERADDR
  is '服务器地址表';
comment on column BASE_SERVERADDR.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_SERVERADDR
  add constraint PK_SERVERADDR primary key (ADDRID);

prompt
prompt Creating table BASE_SERVERADDRQUOT
prompt ==================================
prompt
create table BASE_SERVERADDRQUOT
(
  addrid     INTEGER not null,
  brokercode VARCHAR2(11),
  address    VARCHAR2(128),
  port       INTEGER,
  deleteflag INTEGER
)
;
comment on column BASE_SERVERADDRQUOT.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_SERVERADDRQUOT
  add constraint PK_SERVERADDRQUOT primary key (ADDRID);

prompt
prompt Creating table BASE_SERVERGROUP
prompt ===============================
prompt
create table BASE_SERVERGROUP
(
  serverid   INTEGER not null,
  servername VARCHAR2(20) not null,
  brokerid   INTEGER not null,
  issettle   INTEGER,
  deleteflag INTEGER
)
;
comment on table BASE_SERVERGROUP
  is '服务器组信息';
comment on column BASE_SERVERGROUP.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_SERVERGROUP
  add constraint PK_SERVERGROUP primary key (SERVERID);
create unique index INDEX_SERVERGROUP on BASE_SERVERGROUP (BROKERID, SERVERNAME);

prompt
prompt Creating table BASE_TRADEACCOUNT
prompt ================================
prompt
create table BASE_TRADEACCOUNT
(
  tradeaccountid INTEGER not null,
  tradeaccount   VARCHAR2(20),
  password       VARCHAR2(32),
  brokerid       INTEGER,
  servergroupid  INTEGER,
  alias          VARCHAR2(20),
  realname       VARCHAR2(20),
  contact        VARCHAR2(40),
  idcardtype     INTEGER,
  idcardno       VARCHAR2(20),
  deleteflag     INTEGER
)
;
comment on column BASE_TRADEACCOUNT.idcardtype
  is '证件类型：0 身份证 1 军官证 3 护照 4 其他';
comment on column BASE_TRADEACCOUNT.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_TRADEACCOUNT
  add constraint PK_TRADEACCOUNT primary key (TRADEACCOUNTID);

prompt
prompt Creating table BASE_TRADESTRATEGY
prompt =================================
prompt
create table BASE_TRADESTRATEGY
(
  strategyid INTEGER not null,
  name       VARCHAR2(21),
  frequency  INTEGER,
  addr       VARCHAR2(256),
  deleteflag INTEGER
)
;
comment on table BASE_TRADESTRATEGY
  is '交易策略结构详细表';
comment on column BASE_TRADESTRATEGY.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_TRADESTRATEGY
  add constraint PK_TRADESTRATEGY primary key (STRATEGYID);

prompt
prompt Creating table BASE_USER
prompt ========================
prompt
create table BASE_USER
(
  userid      INTEGER not null,
  account     VARCHAR2(20),
  password    VARCHAR2(32),
  roletype    INTEGER,
  status      INTEGER,
  name        VARCHAR2(20),
  contactinfo VARCHAR2(64),
  assign      INTEGER,
  deleteflag  INTEGER
)
;
comment on table BASE_USER
  is '风控系统用户信息';
comment on column BASE_USER.roletype
  is '角色类型： 0，管理员 1，风控员';
comment on column BASE_USER.status
  is '启用状态：0，未启用 1，启用';
comment on column BASE_USER.contactinfo
  is '联系方式';
comment on column BASE_USER.assign
  is '是否已分配到部门';
comment on column BASE_USER.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table BASE_USER
  add constraint PK_USER primary key (USERID);
create unique index INDEX_RISKUSERACCOUNT on BASE_USER (ACCOUNT);
create unique index PK_USERINFO on BASE_USER (USERID);
insert into BASE_USER (USERID, ACCOUNT, PASSWORD, ROLETYPE, STATUS, NAME, CONTACTINFO, ASSIGN, DELETEFLAG)
values (10846209, 'admin', '21218cca77804d2ba1922c33e0151105', 0, 1, 'admin', '123930303', 0, 0);
prompt
prompt Creating table CFINSTRUEMENTS
prompt =============================
prompt
create table CFINSTRUEMENTS
(
  instrumentid         VARCHAR2(31) not null,
  exchangeid           VARCHAR2(9),
  instrumentname       VARCHAR2(21),
  exchangeinstid       VARCHAR2(31),
  productid            VARCHAR2(31),
  productclass         CHAR(1),
  deliveryyear         INTEGER,
  deliverymonth        INTEGER,
  maxmarketordervolume INTEGER,
  minmarketordervolume INTEGER,
  maxlimitordervolume  INTEGER,
  minlimitordervolume  INTEGER,
  volumemultiple       INTEGER,
  pricetick            FLOAT,
  createdate           VARCHAR2(9),
  opendate             VARCHAR2(9),
  expiredate           VARCHAR2(9),
  startdelivdate       VARCHAR2(9),
  enddelivdate         VARCHAR2(9),
  instlifephase        CHAR(1),
  istrading            INTEGER,
  positiontype         CHAR(1),
  positiondatetype     CHAR(1),
  longmarginratio      FLOAT,
  shortmarginratio     FLOAT
)
;
alter table CFINSTRUEMENTS
  add constraint PK_CFINSTRUEMENTS primary key (INSTRUMENTID);

prompt
prompt Creating table CTP_ORDERS
prompt =========================
prompt
create table CTP_ORDERS
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(31),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          BINARY_DOUBLE,
  volumetotaloriginal INTEGER,
  timecondition       CHAR(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           BINARY_DOUBLE,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  orderlocalid        VARCHAR2(13),
  exchangeid          VARCHAR2(9),
  participantid       VARCHAR2(11),
  clientid            VARCHAR2(11),
  exchangeinstid      VARCHAR2(31),
  traderid            VARCHAR2(21),
  installid           INTEGER,
  ordersubmitstatus   CHAR(1),
  notifysequence      INTEGER,
  tradingday          VARCHAR2(9),
  settlementid        INTEGER,
  ordersysid          VARCHAR2(21),
  ordersource         CHAR(1),
  orderstatus         CHAR(1),
  ordertype           CHAR(1),
  volumetraded        INTEGER,
  volumetotal         INTEGER,
  insertdate          VARCHAR2(9),
  inserttime          VARCHAR2(9),
  activetime          VARCHAR2(9),
  suspendtime         VARCHAR2(9),
  updatetime          VARCHAR2(9),
  canceltime          VARCHAR2(9),
  activetraderid      VARCHAR2(21),
  clearingpartid      VARCHAR2(11),
  sequenceno          INTEGER,
  frontid             INTEGER,
  sessionid           INTEGER,
  userproductinfo     VARCHAR2(11),
  statusmsg           VARCHAR2(81),
  userforceclose      INTEGER,
  activeuserid        VARCHAR2(16),
  brokerorderseq      INTEGER,
  relativeordersysid  VARCHAR2(21),
  avgprice            BINARY_DOUBLE,
  exstatus            INTEGER,
  FTID                INTEGER,
  updateseq           INTEGER,
  validatedate        VARCHAR2(11),
  insertdbtime        DATE
)
;
comment on table CTP_ORDERS
  is '报单';
create index CTP_O_INVESTOR on CTP_ORDERS(INVESTORID);
create index CTP_O_VALIDATEDATE on CTP_ORDERS(VALIDATEDATE);

prompt
prompt Creating table CTP_TRADES
prompt =========================
prompt
create table CTP_TRADES
(
  brokerid           VARCHAR2(11),
  investorid         VARCHAR2(13),
  instrumentid       VARCHAR2(31),
  orderref           VARCHAR2(13),
  userid             VARCHAR2(16),
  exchangeid         VARCHAR2(9),
  tradeid            VARCHAR2(21),
  direction          CHAR(1),
  ordersysid         VARCHAR2(21),
  participantid      VARCHAR2(11),
  clientid           VARCHAR2(11),
  tradingrole        CHAR(1),
  exchangeinstid     VARCHAR2(31),
  offsetflag         CHAR(1),
  hedgeflag          CHAR(1),
  price              BINARY_DOUBLE,
  volume             INTEGER,
  tradedate          VARCHAR2(9),
  tradetime          VARCHAR2(9),
  tradetype          CHAR(1),
  pricesource        CHAR(1),
  traderid           VARCHAR2(21),
  orderlocalid       VARCHAR2(13),
  clearingpartid     VARCHAR2(11),
  businessunit       VARCHAR2(21),
  sequenceno         INTEGER,
  tradingday         VARCHAR2(9),
  settlementid       INTEGER,
  brokerorderseq     INTEGER,
  tradesource        CHAR(1),
  closeprofitbydate  BINARY_DOUBLE,
  closeprofitbytrade BINARY_DOUBLE,
  tradecommission    BINARY_DOUBLE,
  FTID               INTEGER,
  updateseq          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
comment on table CTP_TRADES
  is '成交';
create index CTP_T_BROKER on CTP_TRADES(BROKERID);
create index CTP_T_INVESTOR on CTP_TRADES(INVESTORID);
create index CTP_T_VALIDATEDATE on CTP_TRADES(VALIDATEDATE);

prompt
prompt Creating table EXECUTEIN_ORDERS
prompt ===============================
prompt
create table EXECUTEIN_ORDERS
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(31),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          BINARY_DOUBLE,
  volumetotaloriginal INTEGER,
  timecondition       CHAR(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           BINARY_DOUBLE,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  orderlocalid        VARCHAR2(13),
  exchangeid          VARCHAR2(9),
  participantid       VARCHAR2(11),
  clientid            VARCHAR2(11),
  exchangeinstid      VARCHAR2(31),
  traderid            VARCHAR2(21),
  installid           INTEGER,
  ordersubmitstatus   CHAR(1),
  notifysequence      INTEGER,
  tradingday          VARCHAR2(9),
  settlementid        INTEGER,
  ordersysid          VARCHAR2(21),
  ordersource         CHAR(1),
  orderstatus         CHAR(1),
  ordertype           CHAR(1),
  volumetraded        INTEGER,
  volumetotal         INTEGER,
  insertdate          VARCHAR2(9),
  inserttime          VARCHAR2(9),
  activetime          VARCHAR2(9),
  suspendtime         VARCHAR2(9),
  updatetime          VARCHAR2(9),
  canceltime          VARCHAR2(9),
  activetraderid      VARCHAR2(21),
  clearingpartid      VARCHAR2(11),
  sequenceno          INTEGER,
  frontid             INTEGER,
  sessionid           INTEGER,
  userproductinfo     VARCHAR2(11),
  statusmsg           VARCHAR2(81),
  userforceclose      INTEGER,
  activeuserid        VARCHAR2(16),
  brokerorderseq      INTEGER,
  relativeordersysid  VARCHAR2(21),
  avgprice            BINARY_DOUBLE,
  exstatus            INTEGER,
  FTID                INTEGER,
  updateseq           INTEGER,
  validatedate        VARCHAR2(11),
  insertdbtime        DATE
)
;
comment on table EXECUTEIN_ORDERS
  is '报单';

prompt
prompt Creating table EXECUTEOUT_ORDERS
prompt ================================
prompt
create table EXECUTEOUT_ORDERS
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(31),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          BINARY_DOUBLE,
  volumetotaloriginal INTEGER,
  timecondition       CHAR(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           BINARY_DOUBLE,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  orderlocalid        VARCHAR2(13),
  exchangeid          VARCHAR2(9),
  participantid       VARCHAR2(11),
  clientid            VARCHAR2(11),
  exchangeinstid      VARCHAR2(31),
  traderid            VARCHAR2(21),
  installid           INTEGER,
  ordersubmitstatus   CHAR(1),
  notifysequence      INTEGER,
  tradingday          VARCHAR2(9),
  settlementid        INTEGER,
  ordersysid          VARCHAR2(21),
  ordersource         CHAR(1),
  orderstatus         CHAR(1),
  ordertype           CHAR(1),
  volumetraded        INTEGER,
  volumetotal         INTEGER,
  insertdate          VARCHAR2(9),
  inserttime          VARCHAR2(9),
  activetime          VARCHAR2(9),
  suspendtime         VARCHAR2(9),
  updatetime          VARCHAR2(9),
  canceltime          VARCHAR2(9),
  activetraderid      VARCHAR2(21),
  clearingpartid      VARCHAR2(11),
  sequenceno          INTEGER,
  frontid             INTEGER,
  sessionid           INTEGER,
  userproductinfo     VARCHAR2(11),
  statusmsg           VARCHAR2(81),
  userforceclose      INTEGER,
  activeuserid        VARCHAR2(16),
  brokerorderseq      INTEGER,
  relativeordersysid  VARCHAR2(21),
  avgprice            BINARY_DOUBLE,
  exstatus            INTEGER,
  FTID                INTEGER,
  updateseq           INTEGER,
  validatedate        VARCHAR2(11),
  insertdbtime        DATE
)
;
comment on table EXECUTEOUT_ORDERS
  is '报单';

prompt
prompt Creating table LOG_BROKER
prompt =========================
prompt
create table LOG_BROKER
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on table LOG_BROKER
  is '经纪公司信息日志';
comment on column LOG_BROKER.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_ORGANIZATION
prompt ===============================
prompt
create table LOG_ORGANIZATION
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_ORGANIZATION.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_PRODUCT
prompt ==========================
prompt
create table LOG_PRODUCT
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_PRODUCT.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_RELA_ORG_STRATEGY
prompt ====================================
prompt
create table LOG_RELA_ORG_STRATEGY
(
  data     VARCHAR2(20),
  operater VARCHAR2(64),
  targeter VARCHAR2(64),
  action   VARCHAR2(5),
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;

prompt
prompt Creating table LOG_RELA_ROLE_PRIVELEGE
prompt ======================================
prompt
create table LOG_RELA_ROLE_PRIVELEGE
(
  data     VARCHAR2(20),
  operater VARCHAR2(64),
  targeter VARCHAR2(64),
  action   VARCHAR2(5),
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;

prompt
prompt Creating table LOG_RELA_TRADER_PRODUCT
prompt ======================================
prompt
create table LOG_RELA_TRADER_PRODUCT
(
  data     VARCHAR2(20),
  operater VARCHAR2(64),
  targeter VARCHAR2(64),
  action   VARCHAR2(5),
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;

prompt
prompt Creating table LOG_RELA_USER_ORG
prompt ================================
prompt
create table LOG_RELA_USER_ORG
(
  data     VARCHAR2(20),
  operater VARCHAR2(64),
  targeter VARCHAR2(64),
  action   VARCHAR2(5),
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;

prompt
prompt Creating table LOG_RELA_USER_ROLE
prompt =================================
prompt
create table LOG_RELA_USER_ROLE
(
  data     VARCHAR2(20),
  operater VARCHAR2(64),
  targeter VARCHAR2(64),
  action   VARCHAR2(5),
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;

prompt
prompt Creating table LOG_RELA_USER_TRADEACCOUNT
prompt =========================================
prompt
create table LOG_RELA_USER_TRADEACCOUNT
(
  data     VARCHAR2(20),
  operater VARCHAR2(64),
  targeter VARCHAR2(64),
  action   VARCHAR2(5),
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;

prompt
prompt Creating table LOG_ROLE
prompt =======================
prompt
create table LOG_ROLE
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_ROLE.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_SERVERADDR
prompt =============================
prompt
create table LOG_SERVERADDR
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_SERVERADDR.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_SERVERADDRQUOT
prompt =================================
prompt
create table LOG_SERVERADDRQUOT
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_SERVERADDRQUOT.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_SERVERGROUP
prompt ==============================
prompt
create table LOG_SERVERGROUP
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_SERVERGROUP.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_TRADEACCOUNT
prompt ===============================
prompt
create table LOG_TRADEACCOUNT
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_TRADEACCOUNT.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_TRADESTRATEGY
prompt ================================
prompt
create table LOG_TRADESTRATEGY
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_TRADESTRATEGY.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table LOG_USER
prompt =======================
prompt
create table LOG_USER
(
  data     VARCHAR2(20),
  operater INTEGER,
  targeter INTEGER,
  action   INTEGER,
  content  VARCHAR2(256),
  result   VARCHAR2(101)
)
;
comment on column LOG_USER.action
  is '0：新增      1：删除      2：修改';

prompt
prompt Creating table OFFER_ORDERREFTRANSFER
prompt =====================================
prompt
create table OFFER_ORDERREFTRANSFER
(
  accountid     VARCHAR2(16),
  frontid       INTEGER,
  sessionid     INTEGER,
  orderref      INTEGER,
  ctp_orderref  INTEGER,
  ctp_frontid   INTEGER,
  ctp_sessionid INTEGER,
  time          DATE,
  investorid    VARCHAR2(16),
  brokerid      VARCHAR2(16),
  ordersysid    VARCHAR2(21),
  exchangeid    VARCHAR2(9)
)
;

prompt
prompt Creating table QUOTINFO
prompt =======================
prompt
create table QUOTINFO
(
  tradingday         VARCHAR2(9),
  instrumentid       VARCHAR2(64),
  exchangeid         VARCHAR2(9),
  exchangeinstid     VARCHAR2(31),
  lastprice          BINARY_DOUBLE,
  presettlementprice BINARY_DOUBLE,
  precloseprice      BINARY_DOUBLE,
  preopeninterest    BINARY_DOUBLE,
  openprice          BINARY_DOUBLE,
  highestprice       BINARY_DOUBLE,
  lowestprice        BINARY_DOUBLE,
  volume             INTEGER,
  turnover           BINARY_DOUBLE,
  openinterest       BINARY_DOUBLE,
  closeprice         BINARY_DOUBLE,
  settlementprice    BINARY_DOUBLE,
  upperlimitprice    BINARY_DOUBLE,
  lowerlimitprice    BINARY_DOUBLE,
  predelta           BINARY_DOUBLE,
  currdelta          BINARY_DOUBLE,
  updatetime         VARCHAR2(9),
  updatemillisec     INTEGER,
  bidprice1          BINARY_DOUBLE,
  bidvolume1         INTEGER,
  askprice1          BINARY_DOUBLE,
  askvolume1         INTEGER,
  bidprice2          BINARY_DOUBLE,
  bidvolume2         INTEGER,
  askprice2          BINARY_DOUBLE,
  askvolume2         INTEGER,
  bidprice3          BINARY_DOUBLE,
  bidvolume3         INTEGER,
  askprice3          BINARY_DOUBLE,
  askvolume3         INTEGER,
  bidprice4          BINARY_DOUBLE,
  bidvolume4         INTEGER,
  askprice4          BINARY_DOUBLE,
  askvolume4         INTEGER,
  bidprice5          BINARY_DOUBLE,
  bidvolume5         INTEGER,
  askprice5          BINARY_DOUBLE,
  askvolume5         INTEGER,
  averageprice       BINARY_DOUBLE,
  bidprice6          BINARY_DOUBLE,
  bidvolume6         INTEGER,
  askprice6          BINARY_DOUBLE,
  askvolume6         INTEGER,
  bidprice7          BINARY_DOUBLE,
  bidvolume7         INTEGER,
  askprice7          BINARY_DOUBLE,
  askvolume7         INTEGER,
  bidprice8          BINARY_DOUBLE,
  bidvolume8         INTEGER,
  askprice8          BINARY_DOUBLE,
  askvolume8         INTEGER,
  bidprice9          BINARY_DOUBLE,
  bidvolume9         INTEGER,
  askprice9          BINARY_DOUBLE,
  askvolume9         INTEGER,
  bidprice10         BINARY_DOUBLE,
  bidvolume10        INTEGER,
  askprice10         BINARY_DOUBLE,
  askvolume10        INTEGER,
  newvolume          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
create index INDEX_QT_QUOTINFO_VALIDATEDATE on QUOTINFO (VALIDATEDATE);

prompt
prompt Creating table RELA_ORG_STRATEGY
prompt ================================
prompt
create table RELA_ORG_STRATEGY
(
  strategyid INTEGER not null,
  orgid      INTEGER not null,
  deleteflag INTEGER
)
;
comment on table RELA_ORG_STRATEGY
  is '交易策略与组织机构关系的结构表';
comment on column RELA_ORG_STRATEGY.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table RELA_ORG_STRATEGY
  add constraint PK_RELA_ORG_STRATEGY primary key (STRATEGYID, ORGID);
create unique index PK_RELAORGANDSTRATEGY on RELA_ORG_STRATEGY (STRATEGYID, ORGID);

prompt
prompt Creating table RELA_ROLE_PRIVELEGE
prompt ==================================
prompt
create table RELA_ROLE_PRIVELEGE
(
  roleid      INTEGER not null,
  privelegeid INTEGER not null,
  deleteflag  INTEGER
)
;
comment on column RELA_ROLE_PRIVELEGE.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table RELA_ROLE_PRIVELEGE
  add constraint PK_RELA_ROLE_PRIVELEGE primary key (PRIVELEGEID, ROLEID);

prompt
prompt Creating table RELA_TRADER_PRODUCT
prompt ==================================
prompt
create table RELA_TRADER_PRODUCT
(
  financeproductid INTEGER not null,
  traderid         INTEGER not null,
  deleteflag       INTEGER
)
;
comment on table RELA_TRADER_PRODUCT
  is '理财产品与交易员关系的结构表';
comment on column RELA_TRADER_PRODUCT.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table RELA_TRADER_PRODUCT
  add constraint PK_RELA_TRADER_PRODUCT primary key (FINANCEPRODUCTID, TRADERID);
create unique index PK_RELATRADERANDFPRODUCT on RELA_TRADER_PRODUCT (FINANCEPRODUCTID, TRADERID);

prompt
prompt Creating table RELA_USER_ORG
prompt ============================
prompt
create table RELA_USER_ORG
(
  userid       INTEGER not null,
  orgid        INTEGER not null,
  relationtype INTEGER not null,
  usertype     INTEGER not null,
  deleteflag   INTEGER
)
;
comment on column RELA_USER_ORG.relationtype
  is '0：从属        1：监控';
comment on column RELA_USER_ORG.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table RELA_USER_ORG
  add constraint PK_RELA_USER_ORG primary key (USERID, ORGID, RELATIONTYPE, USERTYPE);
create unique index PK_USERANDORG on RELA_USER_ORG (USERID, ORGID, RELATIONTYPE);

prompt
prompt Creating table RELA_USER_ROLE
prompt =============================
prompt
create table RELA_USER_ROLE
(
  userid     INTEGER not null,
  roleid     INTEGER not null,
  deleteflag INTEGER
)
;
comment on column RELA_USER_ROLE.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table RELA_USER_ROLE
  add constraint PK_RELA_USER_ROLE primary key (USERID, ROLEID);

prompt
prompt Creating table RELA_USER_TRADEACCOUNT
prompt =====================================
prompt
create table RELA_USER_TRADEACCOUNT
(
  userid         INTEGER not null,
  tradeaccountid INTEGER not null,
  caninputorder  INTEGER not null,
  needverify     INTEGER not null,
  deleteflag     INTEGER
)
;
comment on column RELA_USER_TRADEACCOUNT.deleteflag
  is '0 ：没有删除 1：已经删除';
alter table RELA_USER_TRADEACCOUNT
  add constraint PK_RELA_USER_TRADEACCOUNT primary key (USERID, TRADEACCOUNTID, CANINPUTORDER, NEEDVERIFY);
create unique index PK_USERANDTRADEACCOUNT on RELA_USER_TRADEACCOUNT (USERID, TRADEACCOUNTID, CANINPUTORDER);


prompt
prompt Creating table RELA_TRADEACCOUNT_ORG
prompt =====================================
prompt
create table RELA_TRADEACCOUNT_ORG
(
  tradeaccountid INTEGER not null,
  orgid          INTEGER
)
;
alter table RELA_TRADEACCOUNT_ORG
  add constraint PK_RELA_TRADEACCOUNT_ORG primary key (TRADEACCOUNTID);

prompt
prompt Creating table RISK_EVENTMESSAGETEMPLATE
prompt ========================================
prompt
create table RISK_EVENTMESSAGETEMPLATE
(
  riskindicatorid INTEGER,
  risklevelid     INTEGER,
  title           VARCHAR2(40),
  content         VARCHAR2(1000)
)
;

prompt
prompt Creating table RISK_FORCECLOSE_ORDER
prompt ====================================
prompt
create table RISK_FORCECLOSE_ORDER
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(64),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          FLOAT,
  volumetotaloriginal INTEGER,
  timecondition       VARCHAR2(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           FLOAT,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  userforceclose      INTEGER,
  orderclass          CHAR(1),
  straccount          VARCHAR2(32),
  strlocalrequestid   VARCHAR2(32),
  strlocalrefid       VARCHAR2(32),
  strexchange         VARCHAR2(32),
  nfrontid            INTEGER,
  nsessionid          INTEGER,
  szorderref          VARCHAR2(13),
  forceclosetype      INTEGER,
  riskername          VARCHAR2(64),
  insertdate          DATE
)
;

prompt
prompt Creating table RISK_FORCECLOSE_ORDERACTION
prompt ==========================================
prompt
create table RISK_FORCECLOSE_ORDERACTION
(
  brokerid       VARCHAR2(11),
  investorid     VARCHAR2(13),
  orderactionref INTEGER,
  orderref       VARCHAR2(13),
  requestid      INTEGER,
  frontid        INTEGER,
  sessionid      INTEGER,
  exchangeid     VARCHAR2(9),
  ordersysid     VARCHAR2(21),
  actionflag     CHAR(1),
  limitprice     FLOAT,
  volumechange   INTEGER,
  userid         VARCHAR2(16),
  instrumentid   VARCHAR2(31),
  insertdate     DATE,
  riskername     VARCHAR2(64),
  forceclosetype INTEGER
)
;

prompt
prompt Creating table RISK_FUNDNETCALCRESULT
prompt =====================================
prompt
create table RISK_FUNDNETCALCRESULT
(
  tradeaccountid INTEGER,
  innervolumn    FLOAT,
  outervolumn    FLOAT,
  innernetasset  FLOAT,
  outernetasset  FLOAT,
  innerpernet    FLOAT,
  outerpernet    FLOAT,
  totalnetasset  FLOAT,
  updatedate     DATE
)
;

prompt
prompt Creating table RISK_FUNDNETPARAM
prompt ================================
prompt
create table RISK_FUNDNETPARAM
(
  orgid               INTEGER not null,
  innervolumn         FLOAT,
  outervolumn         FLOAT,
  outernetasset       FLOAT,
  innernetassetoption INTEGER
)
;

prompt
prompt Creating table RISK_MESSAGEINFO
prompt ===============================
prompt
create table RISK_MESSAGEINFO
(
  messageid   INTEGER not null,
  title       VARCHAR2(40) not null,
  content     VARCHAR2(1000) not null,
  expireddate DATE not null,
  ownerid     INTEGER,
  createdate  DATE,
  ownername   VARCHAR2(20)
)
;

prompt
prompt Creating table RISK_MESSAGETARGET
prompt =================================
prompt
create table RISK_MESSAGETARGET
(
  riskmgmtuserid INTEGER not null,
  messageid      INTEGER not null,
  account        VARCHAR2(20),
  sendstatus     INTEGER
)
;

prompt
prompt Creating table RISK_ORGIDPLANRELATION
prompt =====================================
prompt
create table RISK_ORGIDPLANRELATION
(
  orgid              INTEGER not null,
  riskindicatorid    INTEGER not null,
  allproduct         INTEGER,
  riskproduct        VARCHAR2(300),
  use                INTEGER,
  risktype           INTEGER,
  begintime          VARCHAR2(6) not null,
  endtime            VARCHAR2(6) not null,
  instruments        VARCHAR2(300),
  margindividdynamic FLOAT,
  marginuse          FLOAT,
  lossamount         FLOAT,
  losspercent        FLOAT,
  planname           VARCHAR2(32)
)
;
alter table RISK_ORGIDPLANRELATION
  add constraint PK_RISK_ORGIDPLANRELATION primary key (ORGID, RISKINDICATORID, BEGINTIME, ENDTIME);

prompt
prompt Creating table RISK_RESPONSE
prompt ============================
prompt
create table RISK_RESPONSE
(
  responsetype INTEGER,
  name         VARCHAR2(40),
  responsedesc VARCHAR2(100)
)
;
insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (1, '限制开仓(单合约)', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (2, '限制开仓', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (4, '限制下单(单合约)', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (16, '限制下单', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (32, '市价强平(单合约)', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (64, '市价强平', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (128, '鸣笛', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (256, '震动', '');

insert into RISK_RESPONSE (RESPONSETYPE, NAME, RESPONSEDESC)
values (512, '鸣笛加震动', '');


prompt
prompt Creating table RISK_RISKEVENT
prompt =============================
prompt
create table RISK_RISKEVENT
(
  riskeventid            INTEGER,
  riskeventsubid         INTEGER,
  tradeaccount           INTEGER,
  eventtime              INTEGER,
  riskindicatorid        INTEGER,
  risklevelid            INTEGER,
  indicatorvalue         FLOAT,
  msgsendstatus          INTEGER,
  isvalid                INTEGER,
  instrumentid           VARCHAR2(64),
  timebegin              VARCHAR2(6),
  timeend                VARCHAR2(6),
  brokerid               VARCHAR2(11),
  indicatorcurrentvalue  FLOAT,
  responsetype           INTEGER,
  indicatorvalue2        FLOAT,
  indicatorcurrentvalue2 FLOAT
)
;

prompt
prompt Creating table RISK_RISKEVENTHANDLING
prompt =====================================
prompt
create table RISK_RISKEVENTHANDLING
(
  riskeventhandlingid INTEGER,
  handlingtime        INTEGER,
  userid              INTEGER,
  riskeventid         INTEGER,
  riskeventsunid      INTEGER,
  handler             VARCHAR2(20),
  accepter            VARCHAR2(20),
  actiontype          INTEGER,
  processstatus       INTEGER,
  eventcomment        VARCHAR2(200)
)
;

prompt
prompt Creating table RISK_RISKINDICATOR
prompt =================================
prompt
create table RISK_RISKINDICATOR
(
  riskindicatorid INTEGER,
  name            VARCHAR2(40),
  algorithmdesc   VARCHAR2(100),
  typename        VARCHAR2(40)
)
;
insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (1, '合约最大持仓', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (2, '市场冲击风险', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (3, '账户限亏', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (4, '今日限亏', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (5, '隔夜跳空', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (6, '保证金比例风险', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (7, '单笔持仓时间', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (8, '基金净值风险', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (9, '交易合约限制', '', '合同级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (10, '保证金使用限制', '', '合同级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (11, '亏损限制', '', '合同级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (12, '单日最大亏损', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (13, '亏损最大值', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (14, '最大回撤值', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (15, '账户持仓合约价值', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (16, '多空轧差合约价值', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (17, '连续亏损天数', '', '公司级风控');

insert into RISK_RISKINDICATOR (RISKINDICATORID, NAME, ALGORITHMDESC, TYPENAME)
values (18, '亏损比例线限制', '', '公司级风控');



prompt
prompt Creating table RISK_RISKWARING
prompt ==============================
prompt
create table RISK_RISKWARING
(
  orgid           INTEGER not null,
  riskindicatorid INTEGER not null,
  risklevelid     INTEGER not null,
  thresholdvalue  FLOAT,
  responsetype    INTEGER,
  color           INTEGER,
  begintime       VARCHAR2(6) not null,
  endtime         VARCHAR2(6) not null
)
;
alter table RISK_RISKWARING
  add constraint PK_RISK_RW primary key (ORGID, RISKINDICATORID, RISKLEVELID, BEGINTIME, ENDTIME)

prompt
prompt Creating table RISK_VERIFYORDER
prompt ===============================
prompt
create table RISK_VERIFYORDER
(
  investorid   VARCHAR2(13),
  instrumentid VARCHAR2(31),
  frontid      INTEGER,
  sessionid    INTEGER,
  orderref     VARCHAR2(13),
  verifyuser   INTEGER,
  verifytime   DATE,
  result       INTEGER
)
;
prompt
prompt Creating table RISK_CONTRACT_SET
prompt ===============================
prompt
CREATE TABLE RISK_CONTRACT_SET  (
   RISKINDICATORID      INTEGER,
   ACCOUNT              VARCHAR2(32),
   "RISKERNAME"         VARCHAR2(32),
   "INSERTDATE"         DATE,
   "PRODUCTID"          VARCHAR2(300),
   "INSTRUMENTS"        VARCHAR2(300),
   "MARGINDIVIDDYNAMIC" FLOAT,
   "MARGINUSE"          FLOAT,
   "LOSSAMOUNT"         FLOAT,
   "LOSSPERCENT"        FLOAT
);
prompt
prompt Creating table RISK_LOSSDAY
prompt ===============================
prompt
create table RISK_LOSSDAY
(
  brokerid         VARCHAR2(25),
  accountid        VARCHAR2(25),
  dynamicprofit    FLOAT,
  lossdaycount     INTEGER,
  lastday          DATE,
  maxdynamicprofit FLOAT
);

prompt
prompt Creating table SETTLEMENTDAY
prompt ============================
prompt
create table SETTLEMENTDAY
(
  settlementday VARCHAR2(11),
  validatedate  DATE
)
;

prompt
prompt Creating table SETTLEMENT_AUTOTIME
prompt ==================================
prompt
create table SETTLEMENT_AUTOTIME
(
  autosettletime VARCHAR2(9),
  validatedate   DATE
)
;

prompt
prompt Creating table SETTLEMENT_COMMISSION
prompt ====================================
prompt
create table SETTLEMENT_COMMISSION
(
  instrumentid            VARCHAR2(31),
  investorrange           CHAR(1),
  brokerid                VARCHAR2(11),
  investorid              VARCHAR2(13),
  openratiobymoney        BINARY_DOUBLE,
  openratiobyvolume       BINARY_DOUBLE,
  closeratiobymoney       BINARY_DOUBLE,
  closeratiobyvolume      BINARY_DOUBLE,
  closetodayratiobymoney  BINARY_DOUBLE,
  closetodayratiobyvolume BINARY_DOUBLE,
  validatedate            VARCHAR2(11),
  insertdbtime            DATE
)
;

prompt
prompt Creating table SETTLEMENT_FUNDINFO
prompt ==================================
prompt
create table SETTLEMENT_FUNDINFO
(
  brokerid               VARCHAR2(11),
  accountid              VARCHAR2(13),
  premortage             BINARY_DOUBLE,
  precredit              BINARY_DOUBLE,
  predeposit             BINARY_DOUBLE,
  prebalance             BINARY_DOUBLE,
  premargin              BINARY_DOUBLE,
  interestbase           BINARY_DOUBLE,
  interest               BINARY_DOUBLE,
  deposit                BINARY_DOUBLE,
  withdraw               BINARY_DOUBLE,
  frozenmargin           BINARY_DOUBLE,
  frozencash             BINARY_DOUBLE,
  frozencommission       BINARY_DOUBLE,
  currmargin             BINARY_DOUBLE,
  cashin                 BINARY_DOUBLE,
  commission             BINARY_DOUBLE,
  closeprofit            BINARY_DOUBLE,
  positionprofit         BINARY_DOUBLE,
  balance                BINARY_DOUBLE,
  available              BINARY_DOUBLE,
  withdrawquota          BINARY_DOUBLE,
  reserve                BINARY_DOUBLE,
  tradingday             VARCHAR2(9),
  settlementid           INTEGER,
  credit                 BINARY_DOUBLE,
  mortgage               BINARY_DOUBLE,
  exchangemargin         BINARY_DOUBLE,
  deliverymargin         BINARY_DOUBLE,
  exchangedeliverymargin BINARY_DOUBLE,
  staticprofit           BINARY_DOUBLE,
  dynamicprofit          BINARY_DOUBLE,
  riskdegree             BINARY_DOUBLE,
  validatedate           VARCHAR2(11),
  insertdbtime           DATE
)
;
comment on table SETTLEMENT_FUNDINFO
  is '今日资金';

prompt
prompt Creating table SETTLEMENT_FUNDINIT
prompt ==================================
prompt
create table SETTLEMENT_FUNDINIT
(
  brokerid               VARCHAR2(11),
  accountid              VARCHAR2(13),
  premortage             BINARY_DOUBLE,
  precredit              BINARY_DOUBLE,
  predeposit             BINARY_DOUBLE,
  prebalance             BINARY_DOUBLE,
  premargin              BINARY_DOUBLE,
  interestbase           BINARY_DOUBLE,
  interest               BINARY_DOUBLE,
  deposit                BINARY_DOUBLE,
  withdraw               BINARY_DOUBLE,
  frozenmargin           BINARY_DOUBLE,
  frozencash             BINARY_DOUBLE,
  frozencommission       BINARY_DOUBLE,
  currmargin             BINARY_DOUBLE,
  cashin                 BINARY_DOUBLE,
  commission             BINARY_DOUBLE,
  closeprofit            BINARY_DOUBLE,
  positionprofit         BINARY_DOUBLE,
  balance                BINARY_DOUBLE,
  available              BINARY_DOUBLE,
  withdrawquota          BINARY_DOUBLE,
  reserve                BINARY_DOUBLE,
  tradingday             VARCHAR2(9),
  settlementid           INTEGER,
  credit                 BINARY_DOUBLE,
  mortgage               BINARY_DOUBLE,
  exchangemargin         BINARY_DOUBLE,
  deliverymargin         BINARY_DOUBLE,
  exchangedeliverymargin BINARY_DOUBLE,
  staticprofit           BINARY_DOUBLE,
  dynamicprofit          BINARY_DOUBLE,
  riskdegree             BINARY_DOUBLE,
  validatedate           VARCHAR2(11),
  insertdbtime           DATE
)
;

prompt
prompt Creating table SETTLEMENT_MARGINRATE
prompt ====================================
prompt
create table SETTLEMENT_MARGINRATE
(
  instrumentid             VARCHAR2(31),
  investorrange            CHAR(1),
  brokerid                 VARCHAR2(11),
  investorid               VARCHAR2(13),
  hedgeflag                CHAR(1),
  longmarginratiobymoney   BINARY_DOUBLE,
  longmarginratiobyvolume  BINARY_DOUBLE,
  shortmarginratiobymoney  BINARY_DOUBLE,
  shortmarginratiobyvolume BINARY_DOUBLE,
  isrelative               INTEGER,
  validatedate             VARCHAR2(11),
  insertdbtime             DATE
)
;

prompt
prompt Creating table SETTLEMENT_OPRATIONRECORD
prompt ========================================
prompt
create table SETTLEMENT_OPRATIONRECORD
(
  userid          INTEGER not null,
  traderid        VARCHAR2(13),
  tradingday      VARCHAR2(11) not null,
  instrumentid    VARCHAR2(31),
  bussinesstype   INTEGER not null,
  actiontype      INTEGER not null,
  oprationitem    VARCHAR2(21),
  originalcontent BINARY_DOUBLE,
  newcontent      BINARY_DOUBLE,
  validatedate    DATE
)
;

prompt
prompt Creating table SETTLEMENT_POSITION
prompt ==================================
prompt
create table SETTLEMENT_POSITION
(
  instrumentid              VARCHAR2(31),
  brokerid                  VARCHAR2(11),
  investorid                VARCHAR2(13),
  posidirection             CHAR(1),
  hedgeflag                 CHAR(1),
  tradingday                VARCHAR2(9),
  positiondate              CHAR(1),
  settlementid              INTEGER,
  position                  INTEGER,
  todayposition             INTEGER,
  ydposition                INTEGER,
  openvolume                INTEGER,
  closevolume               INTEGER,
  openamount                BINARY_DOUBLE,
  closeamount               BINARY_DOUBLE,
  positioncost              BINARY_DOUBLE,
  opencost                  BINARY_DOUBLE,
  longfrozen                INTEGER,
  shortfrozen               INTEGER,
  longfrozenamount          BINARY_DOUBLE,
  shortfrozenamount         BINARY_DOUBLE,
  frozenmargin              BINARY_DOUBLE,
  frozencommission          BINARY_DOUBLE,
  frozencash                BINARY_DOUBLE,
  commission                BINARY_DOUBLE,
  premargin                 BINARY_DOUBLE,
  usemargin                 BINARY_DOUBLE,
  exchangemargin            BINARY_DOUBLE,
  marginratebymoney         BINARY_DOUBLE,
  marginratebyvolume        BINARY_DOUBLE,
  cashin                    BINARY_DOUBLE,
  positionprofit            BINARY_DOUBLE,
  closeprofit               BINARY_DOUBLE,
  closeprofitbydate         BINARY_DOUBLE,
  closeprofitbytrade        BINARY_DOUBLE,
  presettlementprice        BINARY_DOUBLE,
  settlementprice           BINARY_DOUBLE,
  combposition              INTEGER,
  comblongfrozen            INTEGER,
  combshortfrozen           INTEGER,
  positionprofitbytrade     BINARY_DOUBLE,
  totalpositionprofitbydate BINARY_DOUBLE,
  FTID                      INTEGER,
  updateseq                 INTEGER,
  validatedate              VARCHAR2(11),
  insertdbtime              DATE
)
;
comment on table SETTLEMENT_POSITION
  is '持仓';
create index INDEX_SETTPOSITION_INVESTORID on SETTLEMENT_POSITION (INVESTORID);
create index INDEX_SETTPOSITION_TRADINGDAY on SETTLEMENT_POSITION (TRADINGDAY);

prompt
prompt Creating table SETTLEMENT_POSITIONDETAIL
prompt ========================================
prompt
create table SETTLEMENT_POSITIONDETAIL
(
  instrumentid          VARCHAR2(31),
  brokerid              VARCHAR2(11),
  investorid            VARCHAR2(13),
  hedgeflag             CHAR(1),
  direction             CHAR(1),
  opendate              VARCHAR2(9),
  tradeid               VARCHAR2(21),
  volume                INTEGER,
  openprice             BINARY_DOUBLE,
  tradingday            VARCHAR2(9),
  settlementid          INTEGER,
  tradetype             CHAR(1),
  cominstrumentid       VARCHAR2(31),
  exchangeid            VARCHAR2(9),
  closeprofitbydate     BINARY_DOUBLE,
  closeprofitbytrade    BINARY_DOUBLE,
  positionprofitbydate  BINARY_DOUBLE,
  positionprofitbytrade BINARY_DOUBLE,
  margin                BINARY_DOUBLE,
  exchmargin            BINARY_DOUBLE,
  marginratebymoney     BINARY_DOUBLE,
  marginratebyvolume    BINARY_DOUBLE,
  lastsettlementprice   BINARY_DOUBLE,
  settlementprice       BINARY_DOUBLE,
  closevolume           INTEGER,
  closeamount           BINARY_DOUBLE,
  FTID                  INTEGER,
  updateseq             INTEGER,
  validatedate          VARCHAR2(11),
  insertdbtime          DATE
)
;
comment on table SETTLEMENT_POSITIONDETAIL
  is '持仓';
create index INDEX_SETTPOSIDTL_INVESTORID on SETTLEMENT_POSITIONDETAIL (INVESTORID);
create index INDEX_SETTPOSIDTL_TRADINGDAY on SETTLEMENT_POSITIONDETAIL (TRADINGDAY);

prompt
prompt Creating table SETTLEMENT_POSITIONDETAILINIT
prompt ============================================
prompt
create table SETTLEMENT_POSITIONDETAILINIT
(
  instrumentid          VARCHAR2(31),
  brokerid              VARCHAR2(11),
  investorid            VARCHAR2(13),
  hedgeflag             CHAR(1),
  direction             CHAR(1),
  opendate              VARCHAR2(9),
  tradeid               VARCHAR2(21),
  volume                INTEGER,
  openprice             BINARY_DOUBLE,
  tradingday            VARCHAR2(9),
  settlementid          INTEGER,
  tradetype             CHAR(1),
  cominstrumentid       VARCHAR2(31),
  exchangeid            VARCHAR2(9),
  closeprofitbydate     BINARY_DOUBLE,
  closeprofitbytrade    BINARY_DOUBLE,
  positionprofitbydate  BINARY_DOUBLE,
  positionprofitbytrade BINARY_DOUBLE,
  margin                BINARY_DOUBLE,
  exchmargin            BINARY_DOUBLE,
  marginratebymoney     BINARY_DOUBLE,
  marginratebyvolume    BINARY_DOUBLE,
  lastsettlementprice   BINARY_DOUBLE,
  settlementprice       BINARY_DOUBLE,
  closevolume           INTEGER,
  closeamount           BINARY_DOUBLE,
  FTID                  INTEGER,
  updateseq             INTEGER,
  validatedate          VARCHAR2(11),
  insertdbtime          DATE
)
;

prompt
prompt Creating table SETTLEMENT_POSITIONINIT
prompt ======================================
prompt
create table SETTLEMENT_POSITIONINIT
(
  instrumentid              VARCHAR2(31),
  brokerid                  VARCHAR2(11),
  investorid                VARCHAR2(13),
  posidirection             CHAR(1),
  hedgeflag                 CHAR(1),
  tradingday                VARCHAR2(9),
  positiondate              CHAR(1),
  settlementid              INTEGER,
  position                  INTEGER,
  todayposition             INTEGER,
  ydposition                INTEGER,
  openvolume                INTEGER,
  closevolume               INTEGER,
  openamount                BINARY_DOUBLE,
  closeamount               BINARY_DOUBLE,
  positioncost              BINARY_DOUBLE,
  opencost                  BINARY_DOUBLE,
  longfrozen                INTEGER,
  shortfrozen               INTEGER,
  longfrozenamount          BINARY_DOUBLE,
  shortfrozenamount         BINARY_DOUBLE,
  frozenmargin              BINARY_DOUBLE,
  frozencommission          BINARY_DOUBLE,
  frozencash                BINARY_DOUBLE,
  commission                BINARY_DOUBLE,
  premargin                 BINARY_DOUBLE,
  usemargin                 BINARY_DOUBLE,
  exchangemargin            BINARY_DOUBLE,
  marginratebymoney         BINARY_DOUBLE,
  marginratebyvolume        BINARY_DOUBLE,
  cashin                    BINARY_DOUBLE,
  positionprofit            BINARY_DOUBLE,
  closeprofit               BINARY_DOUBLE,
  closeprofitbydate         BINARY_DOUBLE,
  closeprofitbytrade        BINARY_DOUBLE,
  presettlementprice        BINARY_DOUBLE,
  settlementprice           BINARY_DOUBLE,
  combposition              INTEGER,
  comblongfrozen            INTEGER,
  combshortfrozen           INTEGER,
  positionprofitbytrade     BINARY_DOUBLE,
  totalpositionprofitbydate BINARY_DOUBLE,
  FTID                      INTEGER,
  updateseq                 INTEGER,
  validatedate              VARCHAR2(11),
  insertdbtime              DATE
)
;

prompt
prompt Creating table SETTLEMENT_SETTLEMENTPRICE
prompt =========================================
prompt
create table SETTLEMENT_SETTLEMENTPRICE
(
  instrumentid        VARCHAR2(31),
  exchangeid          VARCHAR2(9),
  productid           VARCHAR2(64),
  settlementprice     BINARY_DOUBLE,
  lastsettlementprice BINARY_DOUBLE,
  settlementdate      VARCHAR2(9),
  validatedate        DATE
)
;

prompt
prompt Creating table SETTLEMENT_TRADE
prompt ===============================
prompt
create table SETTLEMENT_TRADE
(
  brokerid           VARCHAR2(11),
  investorid         VARCHAR2(13),
  instrumentid       VARCHAR2(31),
  orderref           VARCHAR2(13),
  userid             VARCHAR2(16),
  exchangeid         VARCHAR2(9),
  tradeid            VARCHAR2(21),
  direction          CHAR(1),
  ordersysid         VARCHAR2(21),
  participantid      VARCHAR2(11),
  clientid           VARCHAR2(11),
  tradingrole        CHAR(1),
  exchangeinstid     VARCHAR2(31),
  offsetflag         CHAR(1),
  hedgeflag          CHAR(1),
  price              BINARY_DOUBLE,
  volume             INTEGER,
  tradedate          VARCHAR2(9),
  tradetime          VARCHAR2(9),
  tradetype          CHAR(1),
  pricesource        CHAR(1),
  traderid           VARCHAR2(21),
  orderlocalid       VARCHAR2(13),
  clearingpartid     VARCHAR2(11),
  businessunit       VARCHAR2(21),
  sequenceno         INTEGER,
  tradingday         VARCHAR2(9),
  settlementid       INTEGER,
  brokerorderseq     INTEGER,
  tradesource        CHAR(1),
  closeprofitbydate  BINARY_DOUBLE,
  closeprofitbytrade BINARY_DOUBLE,
  tradecommission    BINARY_DOUBLE,
  FTID               INTEGER,
  updateseq          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
comment on table SETTLEMENT_TRADE
  is '成交';
create index INDEX_SETTTRADE_INVESTORID on SETTLEMENT_TRADE (INVESTORID);
create index INDEX_SETTTRADE_TRADINGDAY on SETTLEMENT_TRADE (TRADINGDAY);

prompt
prompt Creating table TRADEDATA_COMMISSION
prompt ===================================
prompt
create table TRADEDATA_COMMISSION
(
  instrumentid            VARCHAR2(31),
  investorrange           CHAR(1),
  brokerid                VARCHAR2(11),
  investorid              VARCHAR2(13),
  openratiobymoney        BINARY_DOUBLE,
  openratiobyvolume       BINARY_DOUBLE,
  closeratiobymoney       BINARY_DOUBLE,
  closeratiobyvolume      BINARY_DOUBLE,
  closetodayratiobymoney  BINARY_DOUBLE,
  closetodayratiobyvolume BINARY_DOUBLE,
  validatedate            VARCHAR2(11),
  insertdbtime            DATE
)
;
create index INDEX_COMMISSION_BROKER on TRADEDATA_COMMISSION (BROKERID);
create index INDEX_COMMISSION_INVESTOR on TRADEDATA_COMMISSION (INVESTORID);
create index INDEX_COMMISSION_VALIDATEDATE on TRADEDATA_COMMISSION (VALIDATEDATE);

prompt
prompt Creating table TRADEDATA_FUNDINFO
prompt =================================
prompt
create table TRADEDATA_FUNDINFO
(
  brokerid               VARCHAR2(11),
  accountid              VARCHAR2(13),
  premortage             BINARY_DOUBLE,
  precredit              BINARY_DOUBLE,
  predeposit             BINARY_DOUBLE,
  prebalance             BINARY_DOUBLE,
  premargin              BINARY_DOUBLE,
  interestbase           BINARY_DOUBLE,
  interest               BINARY_DOUBLE,
  deposit                BINARY_DOUBLE,
  withdraw               BINARY_DOUBLE,
  frozenmargin           BINARY_DOUBLE,
  frozencash             BINARY_DOUBLE,
  frozencommission       BINARY_DOUBLE,
  currmargin             BINARY_DOUBLE,
  cashin                 BINARY_DOUBLE,
  commission             BINARY_DOUBLE,
  closeprofit            BINARY_DOUBLE,
  positionprofit         BINARY_DOUBLE,
  balance                BINARY_DOUBLE,
  available              BINARY_DOUBLE,
  withdrawquota          BINARY_DOUBLE,
  reserve                BINARY_DOUBLE,
  tradingday             VARCHAR2(9),
  settlementid           INTEGER,
  credit                 BINARY_DOUBLE,
  mortgage               BINARY_DOUBLE,
  exchangemargin         BINARY_DOUBLE,
  deliverymargin         BINARY_DOUBLE,
  exchangedeliverymargin BINARY_DOUBLE,
  staticprofit           BINARY_DOUBLE,
  dynamicprofit          BINARY_DOUBLE,
  riskdegree             BINARY_DOUBLE,
  validatedate           VARCHAR2(11),
  insertdbtime           DATE
)
;
comment on table TRADEDATA_FUNDINFO
  is '今日资金';
create index INDEX_FUNDINFO_VALIDATEDATE on TRADEDATA_FUNDINFO (VALIDATEDATE);

prompt
prompt Creating table TRADEDATA_FUNDINOUT
prompt ==================================
prompt
create table TRADEDATA_FUNDINOUT
(
  userid       VARCHAR2(21),
  einout       INTEGER,
  volumn       BINARY_DOUBLE,
  opuserid     VARCHAR2(21),
  text         VARCHAR2(200),
  opdate       VARCHAR2(11),
  optime       VARCHAR2(9),
  vaildatedate DATE
)
;
create index INDEX_FUNDINOUT_VALIDATEDATE on TRADEDATA_FUNDINOUT (VAILDATEDATE);

prompt
prompt Creating table TRADEDATA_INITFUND
prompt =================================
prompt
create table TRADEDATA_INITFUND
(
  userid       VARCHAR2(21) not null,
  initfund     BINARY_DOUBLE,
  vaildatedate DATE
)
;
alter table TRADEDATA_INITFUND
  add constraint PK_TRADEDATA_INITFUND primary key (USERID);

prompt
prompt Creating table TRADEDATA_INSTRUEMENTS
prompt =====================================
prompt
create table TRADEDATA_INSTRUEMENTS
(
  instrumentid         VARCHAR2(64),
  exchangeid           VARCHAR2(9),
  instrumentname       VARCHAR2(64),
  exchangeinstid       VARCHAR2(31),
  productid            VARCHAR2(64),
  productclass         CHAR(1),
  deliveryyear         INTEGER,
  deliverymonth        INTEGER,
  maxmarketordervolume INTEGER,
  minmarketordervolume INTEGER,
  maxlimitordervolume  INTEGER,
  minlimitordervolume  INTEGER,
  volumemultiple       INTEGER,
  pricetick            BINARY_DOUBLE,
  createdate           VARCHAR2(9),
  opendate             VARCHAR2(9),
  expiredate           VARCHAR2(9),
  startdelivdate       VARCHAR2(9),
  enddelivdate         VARCHAR2(9),
  instlifephase        CHAR(1),
  istrading            INTEGER,
  positiontype         CHAR(1),
  positiondatetype     CHAR(1),
  longmarginratio      BINARY_DOUBLE,
  shortmarginratio     BINARY_DOUBLE,
  MaxMarginSideAlgorithm        CHAR(1),
  CombMarginDiscountMode        INTEGER,
  UnwindPriorities              INTEGER,
  PriceForMarginOfTodayPosition INTEGER,
  CloseTodayInstructionSupport  INTEGER,
  CloseInstructionSupport       INTEGER,
  Currency                      VARCHAR2(11),
  TicksPerPoint                 INTEGER,
  TickSize                      VARCHAR2(10),
  validatedate         VARCHAR2(11),
  insertdbtime         DATE
)
;

create index INDEX_INSTRUMENT_VALIDATEDATE on TRADEDATA_INSTRUEMENTS (VALIDATEDATE);


prompt
prompt Creating table TRADEDATA_MARGINRATE
prompt ===================================
prompt
create table TRADEDATA_MARGINRATE
(
  instrumentid             VARCHAR2(31),
  investorrange            CHAR(1),
  brokerid                 VARCHAR2(11),
  investorid               VARCHAR2(13),
  hedgeflag                CHAR(1),
  longmarginratiobymoney   BINARY_DOUBLE,
  longmarginratiobyvolume  BINARY_DOUBLE,
  shortmarginratiobymoney  BINARY_DOUBLE,
  shortmarginratiobyvolume BINARY_DOUBLE,
  isrelative               INTEGER,
  validatedate             VARCHAR2(11),
  insertdbtime             DATE
)
;

create index INDEX_MARGIN_BROKER on TRADEDATA_MARGINRATE (BROKERID);
create index INDEX_MARGIN_INVESTOR on TRADEDATA_MARGINRATE (INVESTORID);
create index INDEX_MARGIN_VALIDATEDATE on TRADEDATA_MARGINRATE (VALIDATEDATE);

prompt
prompt Creating table TRADEDATA_ORDERS
prompt ===============================
prompt
create table TRADEDATA_ORDERS
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(31),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          BINARY_DOUBLE,
  volumetotaloriginal INTEGER,
  timecondition       CHAR(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           BINARY_DOUBLE,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  orderlocalid        VARCHAR2(13),
  exchangeid          VARCHAR2(9),
  participantid       VARCHAR2(11),
  clientid            VARCHAR2(11),
  exchangeinstid      VARCHAR2(31),
  traderid            VARCHAR2(21),
  installid           INTEGER,
  ordersubmitstatus   CHAR(1),
  notifysequence      INTEGER,
  tradingday          VARCHAR2(9),
  settlementid        INTEGER,
  ordersysid          VARCHAR2(21),
  ordersource         CHAR(1),
  orderstatus         CHAR(1),
  ordertype           CHAR(1),
  volumetraded        INTEGER,
  volumetotal         INTEGER,
  insertdate          VARCHAR2(9),
  inserttime          VARCHAR2(9),
  activetime          VARCHAR2(9),
  suspendtime         VARCHAR2(9),
  updatetime          VARCHAR2(9),
  canceltime          VARCHAR2(9),
  activetraderid      VARCHAR2(21),
  clearingpartid      VARCHAR2(11),
  sequenceno          INTEGER,
  frontid             INTEGER,
  sessionid           INTEGER,
  userproductinfo     VARCHAR2(11),
  statusmsg           VARCHAR2(81),
  userforceclose      INTEGER,
  activeuserid        VARCHAR2(16),
  brokerorderseq      INTEGER,
  relativeordersysid  VARCHAR2(21),
  avgprice            BINARY_DOUBLE,
  exstatus            INTEGER,
  FTID                INTEGER,
  updateseq           INTEGER,
  validatedate        VARCHAR2(11),
  insertdbtime        DATE
)
;
comment on table TRADEDATA_ORDERS
  is '报单';
create index INDEXINVESTOR on TRADEDATA_ORDERS (INVESTORID);
create index INDEXVALIDATEDATE on TRADEDATA_ORDERS (VALIDATEDATE);

prompt
prompt Creating table TRADEDATA_POSITION
prompt =================================
prompt
create table TRADEDATA_POSITION
(
  instrumentid              VARCHAR2(31),
  brokerid                  VARCHAR2(11),
  investorid                VARCHAR2(13),
  posidirection             CHAR(1),
  hedgeflag                 CHAR(1),
  tradingday                VARCHAR2(9),
  positiondate              CHAR(1),
  settlementid              INTEGER,
  position                  INTEGER,
  todayposition             INTEGER,
  ydposition                INTEGER,
  openvolume                INTEGER,
  closevolume               INTEGER,
  openamount                BINARY_DOUBLE,
  closeamount               BINARY_DOUBLE,
  positioncost              BINARY_DOUBLE,
  opencost                  BINARY_DOUBLE,
  longfrozen                INTEGER,
  shortfrozen               INTEGER,
  longfrozenamount          BINARY_DOUBLE,
  shortfrozenamount         BINARY_DOUBLE,
  frozenmargin              BINARY_DOUBLE,
  frozencommission          BINARY_DOUBLE,
  frozencash                BINARY_DOUBLE,
  commission                BINARY_DOUBLE,
  premargin                 BINARY_DOUBLE,
  usemargin                 BINARY_DOUBLE,
  exchangemargin            BINARY_DOUBLE,
  marginratebymoney         BINARY_DOUBLE,
  marginratebyvolume        BINARY_DOUBLE,
  cashin                    BINARY_DOUBLE,
  positionprofit            BINARY_DOUBLE,
  closeprofit               BINARY_DOUBLE,
  closeprofitbydate         BINARY_DOUBLE,
  closeprofitbytrade        BINARY_DOUBLE,
  presettlementprice        BINARY_DOUBLE,
  settlementprice           BINARY_DOUBLE,
  combposition              INTEGER,
  comblongfrozen            INTEGER,
  combshortfrozen           INTEGER,
  positionprofitbytrade     BINARY_DOUBLE,
  totalpositionprofitbydate BINARY_DOUBLE,
  FTID                      INTEGER,
  updateseq                 INTEGER,
  validatedate              VARCHAR2(11),
  insertdbtime              DATE
)
;
comment on table TRADEDATA_POSITION
  is '持仓';

prompt
prompt Creating table TRADEDATA_POSITIONDETAIL
prompt =======================================
prompt
create table TRADEDATA_POSITIONDETAIL
(
  instrumentid          VARCHAR2(31),
  brokerid              VARCHAR2(11),
  investorid            VARCHAR2(13),
  hedgeflag             CHAR(1),
  direction             CHAR(1),
  opendate              VARCHAR2(9),
  tradeid               VARCHAR2(21),
  volume                INTEGER,
  openprice             BINARY_DOUBLE,
  tradingday            VARCHAR2(9),
  settlementid          INTEGER,
  tradetype             CHAR(1),
  cominstrumentid       VARCHAR2(31),
  exchangeid            VARCHAR2(9),
  closeprofitbydate     BINARY_DOUBLE,
  closeprofitbytrade    BINARY_DOUBLE,
  positionprofitbydate  BINARY_DOUBLE,
  positionprofitbytrade BINARY_DOUBLE,
  margin                BINARY_DOUBLE,
  exchmargin            BINARY_DOUBLE,
  marginratebymoney     BINARY_DOUBLE,
  marginratebyvolume    BINARY_DOUBLE,
  lastsettlementprice   BINARY_DOUBLE,
  settlementprice       BINARY_DOUBLE,
  closevolume           INTEGER,
  closeamount           BINARY_DOUBLE,
  FTID                  INTEGER,
  updateseq             INTEGER,
  validatedate          VARCHAR2(11),
  insertdbtime          DATE
)
;
comment on table TRADEDATA_POSITIONDETAIL
  is '持仓';

prompt
prompt Creating table TRADEDATA_QUTO
prompt =============================
prompt
create table TRADEDATA_QUTO
(
  tradingday         VARCHAR2(9),
  instrumentid       VARCHAR2(64),
  exchangeid         VARCHAR2(9),
  exchangeinstid     VARCHAR2(31),
  lastprice          BINARY_DOUBLE,
  presettlementprice BINARY_DOUBLE,
  precloseprice      BINARY_DOUBLE,
  preopeninterest    BINARY_DOUBLE,
  openprice          BINARY_DOUBLE,
  highestprice       BINARY_DOUBLE,
  lowestprice        BINARY_DOUBLE,
  volume             INTEGER,
  turnover           BINARY_DOUBLE,
  openinterest       BINARY_DOUBLE,
  closeprice         BINARY_DOUBLE,
  settlementprice    BINARY_DOUBLE,
  upperlimitprice    BINARY_DOUBLE,
  lowerlimitprice    BINARY_DOUBLE,
  predelta           BINARY_DOUBLE,
  currdelta          BINARY_DOUBLE,
  updatetime         VARCHAR2(9),
  updatemillisec     INTEGER,
  bidprice1          BINARY_DOUBLE,
  bidvolume1         INTEGER,
  askprice1          BINARY_DOUBLE,
  askvolume1         INTEGER,
  bidprice2          BINARY_DOUBLE,
  bidvolume2         INTEGER,
  askprice2          BINARY_DOUBLE,
  askvolume2         INTEGER,
  bidprice3          BINARY_DOUBLE,
  bidvolume3         INTEGER,
  askprice3          BINARY_DOUBLE,
  askvolume3         INTEGER,
  bidprice4          BINARY_DOUBLE,
  bidvolume4         INTEGER,
  askprice4          BINARY_DOUBLE,
  askvolume4         INTEGER,
  bidprice5          BINARY_DOUBLE,
  bidvolume5         INTEGER,
  askprice5          BINARY_DOUBLE,
  askvolume5         INTEGER,
  averageprice       BINARY_DOUBLE,
  bidprice6          BINARY_DOUBLE,
  bidvolume6         INTEGER,
  askprice6          BINARY_DOUBLE,
  askvolume6         INTEGER,
  bidprice7          BINARY_DOUBLE,
  bidvolume7         INTEGER,
  askprice7          BINARY_DOUBLE,
  askvolume7         INTEGER,
  bidprice8          BINARY_DOUBLE,
  bidvolume8         INTEGER,
  askprice8          BINARY_DOUBLE,
  askvolume8         INTEGER,
  bidprice9          BINARY_DOUBLE,
  bidvolume9         INTEGER,
  askprice9          BINARY_DOUBLE,
  askvolume9         INTEGER,
  bidprice10         BINARY_DOUBLE,
  bidvolume10        INTEGER,
  askprice10         BINARY_DOUBLE,
  askvolume10        INTEGER,
  newvolume          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
create index INDEX_TD_QUTOINFO_VALIDATEDATE on TRADEDATA_QUTO (VALIDATEDATE);
prompt
prompt Creating table TRADEDATA_SETTLEMENTCONFIRM
prompt ==========================================
prompt
create table TRADEDATA_SETTLEMENTCONFIRM
(
  userid       VARCHAR2(21),
  confirmdate  VARCHAR2(11),
  confirmtime  VARCHAR2(9),
  validatedate VARCHAR2(11),
  insertdbtime DATE
)
;

prompt
prompt Creating table TRADEDATA_TRADES
prompt ===============================
prompt
create table TRADEDATA_TRADES
(
  brokerid           VARCHAR2(11),
  investorid         VARCHAR2(13),
  instrumentid       VARCHAR2(31),
  orderref           VARCHAR2(13),
  userid             VARCHAR2(16),
  exchangeid         VARCHAR2(9),
  tradeid            VARCHAR2(21),
  direction          CHAR(1),
  ordersysid         VARCHAR2(21),
  participantid      VARCHAR2(11),
  clientid           VARCHAR2(11),
  tradingrole        CHAR(1),
  exchangeinstid     VARCHAR2(31),
  offsetflag         CHAR(1),
  hedgeflag          CHAR(1),
  price              BINARY_DOUBLE,
  volume             INTEGER,
  tradedate          VARCHAR2(9),
  tradetime          VARCHAR2(9),
  tradetype          CHAR(1),
  pricesource        CHAR(1),
  traderid           VARCHAR2(21),
  orderlocalid       VARCHAR2(13),
  clearingpartid     VARCHAR2(11),
  businessunit       VARCHAR2(21),
  sequenceno         INTEGER,
  tradingday         VARCHAR2(9),
  settlementid       INTEGER,
  brokerorderseq     INTEGER,
  tradesource        CHAR(1),
  closeprofitbydate  BINARY_DOUBLE,
  closeprofitbytrade BINARY_DOUBLE,
  tradecommission    BINARY_DOUBLE,
  FTID               INTEGER,
  updateseq          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
comment on table TRADEDATA_TRADES
  is '成交';
create index INDEXTRADEINVESTOR on TRADEDATA_TRADES (INVESTORID);
create index INDEXTRADEVALIDATEDATE on TRADEDATA_TRADES (VALIDATEDATE);

prompt
prompt Creating table TRADINGDAY
prompt =========================
prompt
create table TRADINGDAY
(
  tradingday   VARCHAR2(11),
  validatedate DATE,
  initstatus   INTEGER
)
;

prompt
prompt Creating table USERMSG_ORIG
prompt ===========================
prompt
create table USERMSG_ORIG
(
  msgid          INTEGER not null,
  msgtype        INTEGER default 0 not null,
  ownerid        INTEGER not null,
  expireddate    INTEGER not null,
  createdatetime VARCHAR2(20),
  title          VARCHAR2(32),
  message        VARCHAR2(512)
)
;
alter table USERMSG_ORIG
  add constraint PK_USERMSGORIG primary key (MSGID);
create index INX1_USERMSGORIG on USERMSG_ORIG (OWNERID, EXPIREDDATE, MSGTYPE);

prompt
prompt Creating table USERMSG_TARGET
prompt =============================
prompt
create table USERMSG_TARGET
(
  targetmsgid  INTEGER not null,
  msgid        INTEGER not null,
  userid       INTEGER not null,
  hassend      INTEGER default 0 not null,
  senddatetime VARCHAR2(20)
)
;
alter table USERMSG_TARGET
  add constraint PK_USERMSGTARGET primary key (TARGETMSGID);



prompt
prompt Creating table TRADEDATA_A_P
prompt =================================
prompt
create table TRADEDATA_A_P
(
  instrumentid              VARCHAR2(31),
  brokerid                  VARCHAR2(11),
  investorid                VARCHAR2(13),
  posidirection             CHAR(1),
  hedgeflag                 CHAR(1),
  tradingday                VARCHAR2(9),
  positiondate              CHAR(1),
  settlementid              INTEGER,
  position                  INTEGER,
  todayposition             INTEGER,
  ydposition                INTEGER,
  openvolume                INTEGER,
  closevolume               INTEGER,
  openamount                BINARY_DOUBLE,
  closeamount               BINARY_DOUBLE,
  positioncost              BINARY_DOUBLE,
  opencost                  BINARY_DOUBLE,
  longfrozen                INTEGER,
  shortfrozen               INTEGER,
  longfrozenamount          BINARY_DOUBLE,
  shortfrozenamount         BINARY_DOUBLE,
  frozenmargin              BINARY_DOUBLE,
  frozencommission          BINARY_DOUBLE,
  frozencash                BINARY_DOUBLE,
  commission                BINARY_DOUBLE,
  premargin                 BINARY_DOUBLE,
  usemargin                 BINARY_DOUBLE,
  exchangemargin            BINARY_DOUBLE,
  marginratebymoney         BINARY_DOUBLE,
  marginratebyvolume        BINARY_DOUBLE,
  cashin                    BINARY_DOUBLE,
  positionprofit            BINARY_DOUBLE,
  closeprofit               BINARY_DOUBLE,
  closeprofitbydate         BINARY_DOUBLE,
  closeprofitbytrade        BINARY_DOUBLE,
  presettlementprice        BINARY_DOUBLE,
  settlementprice           BINARY_DOUBLE,
  combposition              INTEGER,
  comblongfrozen            INTEGER,
  combshortfrozen           INTEGER,
  positionprofitbytrade     BINARY_DOUBLE,
  totalpositionprofitbydate BINARY_DOUBLE,
  FTID                      INTEGER,
  updateseq                 INTEGER,
  validatedate              VARCHAR2(11),
  insertdbtime              DATE
)
;
comment on table TRADEDATA_A_P
  is '账户持仓';
create index INDEX_A_P_VALIDATEDATE on TRADEDATA_A_P(VALIDATEDATE);
create index INDEX_A_P_BROKERID on TRADEDATA_A_P(BROKERID);
create index INDEX_A_P_INVESTORID on TRADEDATA_A_P(INVESTORID);

prompt
prompt Creating table TRADEDATA_A_PD
prompt =======================================
prompt
create table TRADEDATA_A_PD
(
  instrumentid          VARCHAR2(31),
  brokerid              VARCHAR2(11),
  investorid            VARCHAR2(13),
  hedgeflag             CHAR(1),
  direction             CHAR(1),
  opendate              VARCHAR2(9),
  tradeid               VARCHAR2(21),
  volume                INTEGER,
  openprice             BINARY_DOUBLE,
  tradingday            VARCHAR2(9),
  settlementid          INTEGER,
  tradetype             CHAR(1),
  cominstrumentid       VARCHAR2(31),
  exchangeid            VARCHAR2(9),
  closeprofitbydate     BINARY_DOUBLE,
  closeprofitbytrade    BINARY_DOUBLE,
  positionprofitbydate  BINARY_DOUBLE,
  positionprofitbytrade BINARY_DOUBLE,
  margin                BINARY_DOUBLE,
  exchmargin            BINARY_DOUBLE,
  marginratebymoney     BINARY_DOUBLE,
  marginratebyvolume    BINARY_DOUBLE,
  lastsettlementprice   BINARY_DOUBLE,
  settlementprice       BINARY_DOUBLE,
  closevolume           INTEGER,
  closeamount           BINARY_DOUBLE,
  FTID                  INTEGER,
  updateseq             INTEGER,
  validatedate          VARCHAR2(11),
  insertdbtime          DATE
)
;
comment on table TRADEDATA_A_PD
  is '主账户持仓明细';
create index INDEX_A_PD_VALIDATEDATE on TRADEDATA_A_PD(VALIDATEDATE);
create index INDEX_A_PD_BROKERID on TRADEDATA_A_PD(BROKERID);
create index INDEX_A_PD_INVESTORID on TRADEDATA_A_PD(INVESTORID);


prompt
prompt Creating table TRADEDATA_A_FUNDINFO
prompt =================================
prompt
create table TRADEDATA_A_FUNDINFO
(
  brokerid               VARCHAR2(11),
  accountid              VARCHAR2(13),
  premortage             BINARY_DOUBLE,
  precredit              BINARY_DOUBLE,
  predeposit             BINARY_DOUBLE,
  prebalance             BINARY_DOUBLE,
  premargin              BINARY_DOUBLE,
  interestbase           BINARY_DOUBLE,
  interest               BINARY_DOUBLE,
  deposit                BINARY_DOUBLE,
  withdraw               BINARY_DOUBLE,
  frozenmargin           BINARY_DOUBLE,
  frozencash             BINARY_DOUBLE,
  frozencommission       BINARY_DOUBLE,
  currmargin             BINARY_DOUBLE,
  cashin                 BINARY_DOUBLE,
  commission             BINARY_DOUBLE,
  closeprofit            BINARY_DOUBLE,
  positionprofit         BINARY_DOUBLE,
  balance                BINARY_DOUBLE,
  available              BINARY_DOUBLE,
  withdrawquota          BINARY_DOUBLE,
  reserve                BINARY_DOUBLE,
  tradingday             VARCHAR2(9),
  settlementid           INTEGER,
  credit                 BINARY_DOUBLE,
  mortgage               BINARY_DOUBLE,
  exchangemargin         BINARY_DOUBLE,
  deliverymargin         BINARY_DOUBLE,
  exchangedeliverymargin BINARY_DOUBLE,
  staticprofit           BINARY_DOUBLE,
  dynamicprofit          BINARY_DOUBLE,
  riskdegree             BINARY_DOUBLE,
  validatedate           VARCHAR2(11),
  insertdbtime           DATE
)
;
comment on table TRADEDATA_A_FUNDINFO
  is '今日资金';
create index INDEX_A_FUND_VALIDATEDATE on TRADEDATA_A_FUNDINFO(VALIDATEDATE);
create index INDEX_A_FUND_BROKERID on TRADEDATA_A_FUNDINFO(BROKERID);
create index INDEX_A_FUND_ACCOUNTID on TRADEDATA_A_FUNDINFO(ACCOUNTID);


prompt
prompt Creating table TRADEDATA_A_ORDERS
prompt ===============================
prompt
create table TRADEDATA_A_ORDERS
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(31),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          BINARY_DOUBLE,
  volumetotaloriginal INTEGER,
  timecondition       CHAR(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           BINARY_DOUBLE,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  orderlocalid        VARCHAR2(13),
  exchangeid          VARCHAR2(9),
  participantid       VARCHAR2(11),
  clientid            VARCHAR2(11),
  exchangeinstid      VARCHAR2(31),
  traderid            VARCHAR2(21),
  installid           INTEGER,
  ordersubmitstatus   CHAR(1),
  notifysequence      INTEGER,
  tradingday          VARCHAR2(9),
  settlementid        INTEGER,
  ordersysid          VARCHAR2(21),
  ordersource         CHAR(1),
  orderstatus         CHAR(1),
  ordertype           CHAR(1),
  volumetraded        INTEGER,
  volumetotal         INTEGER,
  insertdate          VARCHAR2(9),
  inserttime          VARCHAR2(9),
  activetime          VARCHAR2(9),
  suspendtime         VARCHAR2(9),
  updatetime          VARCHAR2(9),
  canceltime          VARCHAR2(9),
  activetraderid      VARCHAR2(21),
  clearingpartid      VARCHAR2(11),
  sequenceno          INTEGER,
  frontid             INTEGER,
  sessionid           INTEGER,
  userproductinfo     VARCHAR2(11),
  statusmsg           VARCHAR2(81),
  userforceclose      INTEGER,
  activeuserid        VARCHAR2(16),
  brokerorderseq      INTEGER,
  relativeordersysid  VARCHAR2(21),
  avgprice            BINARY_DOUBLE,
  exstatus            INTEGER,
  FTID                INTEGER,
  updateseq           INTEGER,
  validatedate        VARCHAR2(11),
  insertdbtime        DATE
)
;
comment on table TRADEDATA_A_ORDERS
  is '报单';
create index INDEX_A_O_BROKERID on TRADEDATA_A_ORDERS(BROKERID);
create index INDEX_A_O_INVESTOR on TRADEDATA_A_ORDERS(INVESTORID);
create index INDEX_A_O_VALIDATEDATE on TRADEDATA_A_ORDERS(VALIDATEDATE);


prompt
prompt Creating table TRADEDATA_A_TRADES
prompt ===============================
prompt
create table TRADEDATA_A_TRADES
(
  brokerid           VARCHAR2(11),
  investorid         VARCHAR2(13),
  instrumentid       VARCHAR2(31),
  orderref           VARCHAR2(13),
  userid             VARCHAR2(16),
  exchangeid         VARCHAR2(9),
  tradeid            VARCHAR2(21),
  direction          CHAR(1),
  ordersysid         VARCHAR2(21),
  participantid      VARCHAR2(11),
  clientid           VARCHAR2(11),
  tradingrole        CHAR(1),
  exchangeinstid     VARCHAR2(31),
  offsetflag         CHAR(1),
  hedgeflag          CHAR(1),
  price              BINARY_DOUBLE,
  volume             INTEGER,
  tradedate          VARCHAR2(9),
  tradetime          VARCHAR2(9),
  tradetype          CHAR(1),
  pricesource        CHAR(1),
  traderid           VARCHAR2(21),
  orderlocalid       VARCHAR2(13),
  clearingpartid     VARCHAR2(11),
  businessunit       VARCHAR2(21),
  sequenceno         INTEGER,
  tradingday         VARCHAR2(9),
  settlementid       INTEGER,
  brokerorderseq     INTEGER,
  tradesource        CHAR(1),
  closeprofitbydate  BINARY_DOUBLE,
  closeprofitbytrade BINARY_DOUBLE,
  tradecommission    BINARY_DOUBLE,
  FTID               INTEGER,
  updateseq          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
comment on table TRADEDATA_A_TRADES
  is '成交';
create index INDEX_A_T_BROKER on TRADEDATA_A_TRADES(BROKERID);
create index INDEX_A_T_INVESTOR on TRADEDATA_A_TRADES(INVESTORID);
create index INDEX_A_T_VALIDATEDATE on TRADEDATA_A_TRADES(VALIDATEDATE);



prompt
prompt Creating table TRADEDATA_A_P_INIT
prompt =================================
prompt
create table TRADEDATA_A_P_INIT
(
  instrumentid              VARCHAR2(31),
  brokerid                  VARCHAR2(11),
  investorid                VARCHAR2(13),
  posidirection             CHAR(1),
  hedgeflag                 CHAR(1),
  tradingday                VARCHAR2(9),
  positiondate              CHAR(1),
  settlementid              INTEGER,
  position                  INTEGER,
  todayposition             INTEGER,
  ydposition                INTEGER,
  openvolume                INTEGER,
  closevolume               INTEGER,
  openamount                BINARY_DOUBLE,
  closeamount               BINARY_DOUBLE,
  positioncost              BINARY_DOUBLE,
  opencost                  BINARY_DOUBLE,
  longfrozen                INTEGER,
  shortfrozen               INTEGER,
  longfrozenamount          BINARY_DOUBLE,
  shortfrozenamount         BINARY_DOUBLE,
  frozenmargin              BINARY_DOUBLE,
  frozencommission          BINARY_DOUBLE,
  frozencash                BINARY_DOUBLE,
  commission                BINARY_DOUBLE,
  premargin                 BINARY_DOUBLE,
  usemargin                 BINARY_DOUBLE,
  exchangemargin            BINARY_DOUBLE,
  marginratebymoney         BINARY_DOUBLE,
  marginratebyvolume        BINARY_DOUBLE,
  cashin                    BINARY_DOUBLE,
  positionprofit            BINARY_DOUBLE,
  closeprofit               BINARY_DOUBLE,
  closeprofitbydate         BINARY_DOUBLE,
  closeprofitbytrade        BINARY_DOUBLE,
  presettlementprice        BINARY_DOUBLE,
  settlementprice           BINARY_DOUBLE,
  combposition              INTEGER,
  comblongfrozen            INTEGER,
  combshortfrozen           INTEGER,
  positionprofitbytrade     BINARY_DOUBLE,
  totalpositionprofitbydate BINARY_DOUBLE,
  FTID                      INTEGER,
  updateseq                 INTEGER,
  validatedate              VARCHAR2(11),
  insertdbtime              DATE
)
;
comment on table TRADEDATA_A_P_INIT
  is '账户持仓';
create index INDEX_A_PI_VALIDATEDATE on TRADEDATA_A_P_INIT(VALIDATEDATE);
create index INDEX_A_PI_BROKERID on TRADEDATA_A_P_INIT(BROKERID);
create index INDEX_A_PI_INVESTORID on TRADEDATA_A_P_INIT(INVESTORID);

prompt
prompt Creating table TRADEDATA_A_PD_INIT
prompt =======================================
prompt
create table TRADEDATA_A_PD_INIT
(
  instrumentid          VARCHAR2(31),
  brokerid              VARCHAR2(11),
  investorid            VARCHAR2(13),
  hedgeflag             CHAR(1),
  direction             CHAR(1),
  opendate              VARCHAR2(9),
  tradeid               VARCHAR2(21),
  volume                INTEGER,
  openprice             BINARY_DOUBLE,
  tradingday            VARCHAR2(9),
  settlementid          INTEGER,
  tradetype             CHAR(1),
  cominstrumentid       VARCHAR2(31),
  exchangeid            VARCHAR2(9),
  closeprofitbydate     BINARY_DOUBLE,
  closeprofitbytrade    BINARY_DOUBLE,
  positionprofitbydate  BINARY_DOUBLE,
  positionprofitbytrade BINARY_DOUBLE,
  margin                BINARY_DOUBLE,
  exchmargin            BINARY_DOUBLE,
  marginratebymoney     BINARY_DOUBLE,
  marginratebyvolume    BINARY_DOUBLE,
  lastsettlementprice   BINARY_DOUBLE,
  settlementprice       BINARY_DOUBLE,
  closevolume           INTEGER,
  closeamount           BINARY_DOUBLE,
  FTID                  INTEGER,
  updateseq             INTEGER,
  validatedate          VARCHAR2(11),
  insertdbtime          DATE
)
;
comment on table TRADEDATA_A_PD_INIT
  is '主账户持仓明细';
create index INDEX_A_PDI_VALIDATEDATE on TRADEDATA_A_PD_INIT(VALIDATEDATE);
create index INDEX_A_PDI_BROKERID on TRADEDATA_A_PD_INIT(BROKERID);
create index INDEX_A_PDI_INVESTORID on TRADEDATA_A_PD_INIT(INVESTORID);


prompt
prompt Creating table TRADEDATA_A_FUNDINIT
prompt =================================
prompt
create table TRADEDATA_A_FUNDINIT
(
  brokerid               VARCHAR2(11),
  accountid              VARCHAR2(13),
  premortage             BINARY_DOUBLE,
  precredit              BINARY_DOUBLE,
  predeposit             BINARY_DOUBLE,
  prebalance             BINARY_DOUBLE,
  premargin              BINARY_DOUBLE,
  interestbase           BINARY_DOUBLE,
  interest               BINARY_DOUBLE,
  deposit                BINARY_DOUBLE,
  withdraw               BINARY_DOUBLE,
  frozenmargin           BINARY_DOUBLE,
  frozencash             BINARY_DOUBLE,
  frozencommission       BINARY_DOUBLE,
  currmargin             BINARY_DOUBLE,
  cashin                 BINARY_DOUBLE,
  commission             BINARY_DOUBLE,
  closeprofit            BINARY_DOUBLE,
  positionprofit         BINARY_DOUBLE,
  balance                BINARY_DOUBLE,
  available              BINARY_DOUBLE,
  withdrawquota          BINARY_DOUBLE,
  reserve                BINARY_DOUBLE,
  tradingday             VARCHAR2(9),
  settlementid           INTEGER,
  credit                 BINARY_DOUBLE,
  mortgage               BINARY_DOUBLE,
  exchangemargin         BINARY_DOUBLE,
  deliverymargin         BINARY_DOUBLE,
  exchangedeliverymargin BINARY_DOUBLE,
  staticprofit           BINARY_DOUBLE,
  dynamicprofit          BINARY_DOUBLE,
  riskdegree             BINARY_DOUBLE,
  validatedate           VARCHAR2(11),
  insertdbtime           DATE
)
;
comment on table TRADEDATA_A_FUNDINIT
  is '今日资金';
create index INDEX_A_FI_VALIDATEDATE on TRADEDATA_A_FUNDINIT(VALIDATEDATE);
create index INDEX_A_FI_BROKERID on TRADEDATA_A_FUNDINIT(BROKERID);
create index INDEX_A_FI_ACCOUNTID on TRADEDATA_A_FUNDINIT(ACCOUNTID);



prompt
prompt Creating table TRADEDATA_A_O_INIT
prompt ===============================
prompt
create table TRADEDATA_A_O_INIT
(
  brokerid            VARCHAR2(11),
  investorid          VARCHAR2(13),
  instrumentid        VARCHAR2(31),
  orderref            VARCHAR2(13),
  userid              VARCHAR2(16),
  orderpricetype      CHAR(1),
  direction           CHAR(1),
  comboffsetflag      VARCHAR2(5),
  combhedgeflag       VARCHAR2(5),
  limitprice          BINARY_DOUBLE,
  volumetotaloriginal INTEGER,
  timecondition       CHAR(1),
  gtddate             VARCHAR2(9),
  volumecondition     CHAR(1),
  minvolume           INTEGER,
  contingentcondition CHAR(1),
  stopprice           BINARY_DOUBLE,
  forceclosereason    CHAR(1),
  isautosuspend       INTEGER,
  businessunit        VARCHAR2(21),
  requestid           INTEGER,
  orderlocalid        VARCHAR2(13),
  exchangeid          VARCHAR2(9),
  participantid       VARCHAR2(11),
  clientid            VARCHAR2(11),
  exchangeinstid      VARCHAR2(31),
  traderid            VARCHAR2(21),
  installid           INTEGER,
  ordersubmitstatus   CHAR(1),
  notifysequence      INTEGER,
  tradingday          VARCHAR2(9),
  settlementid        INTEGER,
  ordersysid          VARCHAR2(21),
  ordersource         CHAR(1),
  orderstatus         CHAR(1),
  ordertype           CHAR(1),
  volumetraded        INTEGER,
  volumetotal         INTEGER,
  insertdate          VARCHAR2(9),
  inserttime          VARCHAR2(9),
  activetime          VARCHAR2(9),
  suspendtime         VARCHAR2(9),
  updatetime          VARCHAR2(9),
  canceltime          VARCHAR2(9),
  activetraderid      VARCHAR2(21),
  clearingpartid      VARCHAR2(11),
  sequenceno          INTEGER,
  frontid             INTEGER,
  sessionid           INTEGER,
  userproductinfo     VARCHAR2(11),
  statusmsg           VARCHAR2(81),
  userforceclose      INTEGER,
  activeuserid        VARCHAR2(16),
  brokerorderseq      INTEGER,
  relativeordersysid  VARCHAR2(21),
  avgprice            BINARY_DOUBLE,
  exstatus            INTEGER,
  FTID                INTEGER,
  updateseq           INTEGER,
  validatedate        VARCHAR2(11),
  insertdbtime        DATE
)
;
comment on table TRADEDATA_A_O_INIT
  is '报单';
create index INDEX_A_OI_BROKERID on TRADEDATA_A_O_INIT(BROKERID);
create index INDEX_A_OI_INVESTOR on TRADEDATA_A_O_INIT(INVESTORID);
create index INDEX_A_OI_VALIDATEDATE on TRADEDATA_A_O_INIT(VALIDATEDATE);


prompt
prompt Creating table TRADEDATA_A_T_INIT
prompt ===============================
prompt
create table TRADEDATA_A_T_INIT
(
  brokerid           VARCHAR2(11),
  investorid         VARCHAR2(13),
  instrumentid       VARCHAR2(31),
  orderref           VARCHAR2(13),
  userid             VARCHAR2(16),
  exchangeid         VARCHAR2(9),
  tradeid            VARCHAR2(21),
  direction          CHAR(1),
  ordersysid         VARCHAR2(21),
  participantid      VARCHAR2(11),
  clientid           VARCHAR2(11),
  tradingrole        CHAR(1),
  exchangeinstid     VARCHAR2(31),
  offsetflag         CHAR(1),
  hedgeflag          CHAR(1),
  price              BINARY_DOUBLE,
  volume             INTEGER,
  tradedate          VARCHAR2(9),
  tradetime          VARCHAR2(9),
  tradetype          CHAR(1),
  pricesource        CHAR(1),
  traderid           VARCHAR2(21),
  orderlocalid       VARCHAR2(13),
  clearingpartid     VARCHAR2(11),
  businessunit       VARCHAR2(21),
  sequenceno         INTEGER,
  tradingday         VARCHAR2(9),
  settlementid       INTEGER,
  brokerorderseq     INTEGER,
  tradesource        CHAR(1),
  closeprofitbydate  BINARY_DOUBLE,
  closeprofitbytrade BINARY_DOUBLE,
  tradecommission    BINARY_DOUBLE,
  FTID               INTEGER,
  updateseq          INTEGER,
  validatedate       VARCHAR2(11),
  insertdbtime       DATE
)
;
comment on table TRADEDATA_A_T_INIT
  is '成交';
create index INDEX_A_TI_BROKER on TRADEDATA_A_T_INIT(BROKERID);
create index INDEX_A_TI_INVESTOR on TRADEDATA_A_T_INIT(INVESTORID);
create index INDEX_A_TI_VALIDATEDATE on TRADEDATA_A_T_INIT(VALIDATEDATE);


prompt
prompt Creating table STRATEGY_INDEX
prompt ===============================
prompt
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
  insertdbtime DATE
);
alter table STRATEGY_INDEX
  add constraint PK_STRATEGY_INDEX primary key (indexname, tradername);


prompt
prompt Creating table STRATEGY_INDEX_RELATION
prompt ===============================
prompt
create table STRATEGY_INDEX_RELATION
(
  strategyname  VARCHAR2(25),
  strategyindex VARCHAR2(25)
);
alter table STRATEGY_INDEX_RELATION
  add constraint PK_STRATEGY_INDEX_RELATION primary key (strategyname, strategyindex);

  
prompt
prompt Creating table STRATEGY_INSTANCE
prompt ===============================
prompt
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
  timerspan             INTEGER
);
alter table STRATEGY_INSTANCE
  add constraint PK_STRATEGY_INSTANCE primary key (strategyname, tradername);

  
prompt
prompt Creating table STRATEGY_STRATEGY
prompt ===============================
prompt
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
  insertdbtime     DATE
);
alter table STRATEGY_STRATEGY
  add constraint PK_STRATEGY_STRATEGY primary key (strategyname, tradername);
  

  


spool off
