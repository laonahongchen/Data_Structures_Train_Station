# 总体：

- 在每个页面的context里均需要传入login_name和authority两个值。其中login_name为登录名，authority为权限，权限与作业文档中一致。

- 没有涉及到的html均不用从py中引出，是html的某些模板。

# 具体到app
- trains应该只有管理员才可以访问

# 具体到html

## login.html
- 用于登录

- 会额外传出inputName和inputPassword两个内容用于登录。

## signup.html

- 用于注册

- 会传出inputEmail、inputName、inputPassword、inputPhone四个内容用于注册。

## SeekTrain.html

- 用于展示查找的车次，本页面同时兼顾查询直达和有中转的。

- 需要传入asked(True, False)。代表这个页面是否是查询过的页面。

- 需要传入Trains为set套set的形式，里层是每个火车的具体参数，外层是每辆火车。

- 会传出from, destination, dateoftrain用于查询，以及yzz的一个checkbox勾选选项帮助判断是否有中转

- 买票操作需要再完成

## add_train*.html

- 需要依次进入add_train add_train_in_class add_train_in_station三个html，每次submit后需要跳转到下一个html。

- 并在后两个中用到两个枚举变量num_price和num_station。

- 在add_train_in_station中需要有class_train这样一个set，从而保证其正确。

- add_train 传出 trainid,trainname,catalogs,num_station,num_price

- add_train_in_class 传出class_train表示等级的名字

- add_train_in_station 传出name_station time_arriv time_start time_stopover price[][] 这些值
