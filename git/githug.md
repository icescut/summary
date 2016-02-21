# githug
一个练习Git的程序

## init

>题目：  
A new directory, `git_hug`, has been created; initialize an empty repository in it.  
>解答：  
git init

## config
其实也可直接修改home目录下的.gitconfig文件，命令别名也放在这个文件中。
>题目：  
Set up your git name and email, this is important so that your commits can be identified.  
>解答：  
git config --global user.name 你的名字  
git config --global user.email 你的邮箱

## add
* `git add -A`把所有更改添加到index，包括新增、修改、删除
* `git add .`把新增、修改添加到index
* `git add -u`把修改删除、添加到index

>题目：  
There is a file in your folder called `README`, you should add it to your staging area.  
>解答：  
git add README

## commit

>题目：  
The `README` file has been added to your staging area, now commit it.  
>解答：  
git commit

## clone

>题目：  
Clone the repository at https://github.com/Gazler/cloneme.   
>解答：  
git clone https://github.com/Gazler/cloneme.   

## clone_to_folder

>题目：  
Clone the repository at https://github.com/Gazler/cloneme to `my_cloned_repo`.  
>解答：  
git clone https://github.com/Gazler/cloneme my_cloned_repo 

## ignore
可以使用unix的glob。
* `*.html`，忽略所有html文件
* `!index.html`，不忽略index.html
* `*.[oa]`，忽略所有C语言的中间文件
* `test\`，忽略test文件夹

>题目：  
The text editor 'vim' creates files ending in `.swp` (swap files) for all files that are currently open.  We don't want them creeping into the repository.  Make this repository ignore `.swp` files.  
>解答：  
在.gitignore文件最后一行加上*.swp

## include

>题目：  
Notice a few files with the '.a' extension.  We want git to ignore all but the 'lib.a' file.  
>解答：  
在.gitignore文件最后加上两行*.a和!lib.a

## status
git st是很重要的命令，使用以查看状态更新并且会有非常有用的提示。

>题目:  
There are some files in this repository, one of the files is untracked, which file is it?  
>解答：  
git st
回答红色高亮的文件

## number\_of\_files\_committed
题目的标题应该为to\_be\_committed比较合适
>题目：  
There are some files in this repository, how many of the files will be committed?  
>解答：  
git st
// 回答绿色的文件的个数

## rm
`git rm`用于删除文件并从index中删除，commit后才会真正更新仓库
>题目：  
A file has been removed from the working tree, however the file was not removed from the repository.  Find out what this file was and remove it.  
>解答：  
git rm deleteme.rb  

## rm_cached
不小心把文件加到index，使用`git rm --cached`从index中删除但不删除文件。
>题目：  
A file has accidentally been added to your staging area, find out which file and remove it from the staging area.  *NOTE* Do not remove the file from the file system, only from git.  
>解答：  
git rm --cached deleteme.rb

## stash
使用`git stash`保存当前工作现场，最好不要用`git stash`，因为很容易忘记。直接创建分支保存比较好。
>题目：  
You've made some changes and want to work on them later. You should save them, but don't commit them.  
>解答：  
git stash

## rename
与linux命令类似，`mv`除了具有移动的作用还能重命名。
>题目：  
We have a file called `oldfile.txt`. We want to rename it to `newfile.txt` and stage this change.  
>解答：  
git mv oldfile.txt newfile.txt

## restructure
不知为何在windows上不能用*通配
>题目：  
You added some files to your repository, but now realize that your project needs to be restructured.  Make a new folder named `src` and using Git move all of the .html files into this folder.  
>解答：  
mkdir src  
git mv about.html src/  
...

## log

>题目：  
You will be asked for the hash of most recent commit.  You will need to investigate the logs of the repository for this.  
>解答：  
git log

## tag

>题目：  
We have a git repo and we want to tag the current commit with `new_tag`.  
>解答：  
git tag new_tag  

## push_tag

>题目：  
There are tags in the repository that aren't pushed into remote repository. Push them now.  
>解答：  
git push --tags

## commit_amend

>题目：  
The `README` file has been committed, but it looks like the file `forgotten_file.rb` was missing from the commit.  Add the file and amend your previous commit to include it.  
>解答：  
git add forgotten_file.rb  
git commit --amend

## commit_in_future
日期格式可以通过`git log`复制。
>题目：  
Commit your changes with the future date (e.g. tomorrow).  
>解答：  
git commit --date="Wed Feb 11 03:21:39 2016 +0800"

## reset
reset不会重写working tree, 除非使用--hard参数。
>题目：  
There are two files to be committed.  The goal was to add each file as a separate commit, however both were added by accident.  Unstage the file `to_commit_second.rb` using the reset command (don't commit anything).  
>解答：  
git reset HEAD to\_commit\_second.rb  

## reset_soft
* soft只重置HEAD指针，不改变index、工作区
* mixed重置HEAD指针、index，不改变工作区
* hard重置HEAD指针、index、工作区

>题目：  
You committed too soon. Now you want to undo the last commit, while keeping the index.  
>解答：  
git reset HEAD~1 --soft

## checkout_file
reset主要用于回退版本，而checkout主要用于重写working tree。
>题目：  
A file has been modified, but you don't want to keep the modification.  Checkout the `config.rb` file from the last commit.  
>解答：  
git checkout -- config.rb 

## remote

>题目：  
This project has a remote repository.  Identify it.  
>解答：  
git remote  

## remote_url

>题目：  
The remote repositories have a url associated to them.  Please enter the url of remote_location.  
>解答：  
git remote -v  

## pull

>题目：  
You need to pull changes from your origin repository.  
>解答：  
git pull origin master

## remote_add

>题目：  
Add a remote repository called `origin` with the url https://github.com/githug/githug  
>解答：  
git remote add origin https://github.com/githug/githug

## push 

>题目：  
Your local master branch has diverged from the remote origin/master branch. Rebase your commit onto origin/master and push it to remote.  
>解答：  
git rebase origin/master  
git push origin

## diff
* git diff 不指定任何变量，为比较working tree与index
* git diff --cached 不指定任何变量，为比较index与HEAD
* git diff HEAD 为比较working tree与HEAD
* git diff topic master或git diff topic..master 为比较两个分支
* git diff topic...master 为比较分别开发以来，master分支上的changed
* git diff test 为比较当前目录与test分支的差别
* git diff HEAD^ HEAD 为比较上次与上上次提交
* git diff SHA1 SHA2 为比较两个历史版本

>题目：  
There have been modifications to the `app.rb` file since your last commit.  Find out which line has changed.  
>解答：   
git diff app.rb

## blame

>题目：  
Someone has put a password inside the file `config.rb` find out who it was.  
>解答：   
git blame config.rb  

## branch 

>题目：  
You want to work on a piece of code that has the potential to break things, create the branch test_code.  
>解答：   
git checkout -b test_code  

## checkout 

>题目：  
Create and switch to a new branch called my_branch.  You will need to create a branch like you did in the previous level.  
>解答：   
git checkout -b my_branch  

## checkout\_tag

>题目：  
You need to fix a bug in the version 1.2 of your app. Checkout the tag `v1.2`.  
>解答：  
git checkout v1.2  

## checkout\_tag\_over\_branch

>题目：  
You need to fix a bug in the version 1.2 of your app. Checkout the tag `v1.2` (Note: There is also a branch named `v1.2`).    
>解答：   
git checkout tags/v1.2  

## branch\_at

>题目：  
You forgot to branch at the previous commit and made a commit on top of it. Create branch test_branch at the commit before the last.  
>解答：   
git branch test\_branch 8ad75c14a423dfeda5686e7f4648a15e17373ce2  

## delete\_branch

>题目：  
You have created too many branches for your project. There is an old branch in your repo called 'delete_me', you should delete it.  
>解答：   
git branch -D delete_me

## push\_branch

>题目：  
You've made some changes to a local branch and want to share it, but aren't yet ready to merge it with the 'master' branch.  Push only 'test_branch' to the remote repository  
>解答：  
git push origin test_branch

## merge

>题目：  
We have a file in the branch 'feature'; Let's merge it to the master branch.  
>解答：  
git merge feature

## fetch

>题目：  
Looks like a new branch was pushed into our remote repository. Get the changes without merging them with the local repository  
>解答：  
git fetch  

## rebase  
注意顺序，本题是以master为基础，延伸feature
>题目：  
We are using a git rebase workflow and the feature branch is ready to go into master. Let's rebase the feature branch onto our master branch.  
>解答：  
git checkout feature  
git rebase master  

## repack
-d参数表示删除redundant
>题目：  
Optimise how your repository is packaged ensuring that redundant packs are removed.  
>解答：  
git repack -d

## cherry-pick

>题目：  
Your new feature isn't worth the time and you're going to delete it. But it has one commit that fills in `README` file, and you want this commit to be on the master as well.  
>解答：  
// 先查看new-feature分支的commit id  
git log new-feature  
// 把该commit提交到master分支上  
git cherry-pick ca32a6dac7b6f97975edbe19a4296c2ee7682f68

## grep

>题目：  
Your project's deadline approaches, you should evaluate how many TODOs are left in your code  
>解答：   
git grep TODO 

## rename\_commit

>题目：  
Correct the typo in the message of your first (non-root) commit.  
>解答：  
// 回退前两个版本，需要修改的改为edit   
git rebase -i HEAD~2  
// 修改注释  
git commit --amend  
// 继续执行  
git rebase --continue  

## squash

>题目：  
Correct the typo in the message of your first (non-root) commit.  
>解答：  
// 回退前两个版本，需要合并的的改为squash
git rebase -i HEAD~4

## merge\_squash
>题目：  
Merge all commits from the long-feature-branch as a single commit.  
>解答：  
git merge --squash long-feature-branch
git commit

## reorder

>题目：  
You have committed several times but in the wrong order. Please reorder your commits.  
>解答：  
回退前两个版本，修改顺序
git rebase -i HEAD~2 

## bisect
1. git bisect start  
2. git bisect good <commit id> 设置正常的点
3. git bisect bad <commit id> 设置不正常的点
4. 根据测试结果输入git bisect good或bad，直到提示哪个是第一个出错的地方

>题目：  
A bug was introduced somewhere along the way.  You know that running `ruby prog.rb 5` should output 15.  You can also run `make test`.  What are the first 7 chars of the hash of the commit that introduced the bug.  
>解答：  
git bisect start  
git bisect good f608824888b83bbedc1f658be7496ffea467a8fb  
git bisect bad master   
ruby prog.rb 5 // -->15  
git bisect good  
ruby prog.rb 5 // -->11  
git bisect bad   
ruby prog.rb 5 // -->15   
git bisect good  
ruby prog.rb 5 // -->15   
git bisect good  
// 提示18ed2ac1522a014412d4303ce7c8db39becab076 is the first bad commit  

## stage\_lines

git add 交互模式的参数
```
y - stage this hunk
n - do not stage this hunk
a - stage this and all the remaining hunks in the file
d - do not stage this hunk nor any of the remaining hunks in the file
g - select a hunk to go to
/ - search for a hunk matching the given regex
j - leave this hunk undecided, see next undecided hunk
J - leave this hunk undecided, see next hunk
k - leave this hunk undecided, see previous undecided hunk
K - leave this hunk undecided, see previous hunk
s - split the current hunk into smaller hunks
e - manually edit the current hunk
? - print help
```

>题目：  
You've made changes within a single file that belong to two different features, but neither of the changes are yet staged. Stage only the changes belonging to the first feature.  
>解答：  
git add -p  
// 选择e，删除第二行

## find\_old\_branch

>题目：  
You have been working on a branch but got distracted by a major issue and forgot the name of it. Switch back to that branch.  
>解答：  
git reflog  
git checkout solve\_world\_hunger

## revert
revert与reset不同在于，revert把撤销操作为一次新的提交

>题目：  
You have committed several times but want to undo the middle commit.
All commits have been pushed, so you can't change existing history.  
>解答：   
git revert HEAD^

## restore 

>题目：  
You decided to delete your latest commit by running `git reset --hard HEAD^`.  (Not a smart thing to do.)  You then change your mind, and want that commit back.  Restore the deleted commit.  
>解答：   
git reflog  
git reset --hard db045a6

## conflict

>题目：  
You need to merge mybranch into the current branch (master). But there may be some incorrect changes in mybranch which may cause conflicts. Solve any merge-conflicts you come across and finish the merge.  
>解答：  
git merge mybranch   
vim poem.txt  
git add -A  
git commit  

## submodule

>题目：  
You want to include the files from the following repo: `https://github.com/jackmaney/githug-include-me` into a the folder `./githug-include-me`. Do this without cloning the repo or copying the files from the repo into this repo.  
>解答：  
git submodule add https://github.com/jackmaney/githug-include-me ./githug-include-me

## contribute

>题目：  
This is the final level, the goal is to contribute to this repository by making a pull request on GitHub.  Please note that this level is designed to encourage you to add a valid contribution to Githug, not testing your ability to create a pull request.  Contributions that are likely to be accepted are levels, bug fixes and improved documentation.  
>解答：  
使用github工作流，先Fork，再pull request。
