git clone https://github.com/LiuZzhao/test.git正克隆到 'test'...Username for 'https://github.com': LiuZzhao Password for 'https://LiuZzhao@github.com': remote: Support for password authentication was removed on August 13, 2021.remote: Please see https://docs.github.com/get-started/getting-started-with-git/about-remote-repositories#cloning-with-https-urls for information on currently recommended modes of authentication.fatal: 'https://github.com/LiuZzhao/test.git/' 鉴权失败

请确保您已按照之前提供的方法生成了个人访问令牌，并将其用作密码。以下是再次生成个人访问令牌的步骤：

1. 登录到您的 GitHub 帐户。

2. 点击右上角的头像，然后选择 "Settings"（设置）。

3. 在左侧导航栏中，选择 "Developer settings"（开发者设置）。

4. 在下拉菜单中选择 "Personal access tokens"（个人访问令牌）。

5. 点击 "Generate new token"（生成新令牌）。

6. 在 "Note"（备注）字段中，输入一个描述此令牌用途的名称。

7. 在 "Select scopes"（选择范围）中选择此令牌需要的权限。对于克隆私有存储库，至少需要选择 "repo" 范围。

8. 点击 "Generate token"（生成令牌）。

9. GitHub 将生成一个个人访问令牌，并显示在页面上。请务必复制此令牌，并在稍后使用时妥善保存。一旦您离开该页面，您将无法再次看到此令牌的值。

10. 将生成的个人访问令牌粘贴到 git 命令提示输入密码时的密码字段中。



*** 请告诉我您是谁。

运行

  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"



生成ssh

ssh-keygen -t rsa -b 4096 -C "lz363567051@163.com"

git add .
git commit -m "XXXX"
git push

