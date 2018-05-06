#总体：
----在每个页面的context里均需要传入login_name和authority两个值。其中login_name为登录名，authority为权限，权限与作业文档中一致。
----没有涉及到的html均不用从py中引出，是html的某些模板。

#具体到app
----trains应该只有管理员才可以访问

#具体到html
##login.html:
----用于登录
----会额外传出inputName和inputPassword两个内容用于登录。

##signup.html:
----用于注册
----会传出inputEmail、inputName、inputPassword、inputPhone四个内容用于注册。

##SeekTrain.html:
----用于展示查找的车次，本页面同时兼顾查询直达和有中转的。
----需要传入asked(True, False)。代表这个页面是否是查询过的页面。
----需要传入Trains为list套list的形式，里层是每个火车的具体参数，外层是每辆火车。
----会传出from, destination, dateoftrain用于查询，页面里会有2个submit，value分别为"zjd"和"yzz"代表直接到和有中转
