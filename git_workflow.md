#git workflow

This GitHub uses a [forking workflow](https://www.atlassian.com/git/workflows#!workflow-forking), where richelbilderbeek h is GitHub is used for intergration of the forks.

The workflow has multiple phases:
- Setting up
- Adding changes
- Updating from the inegration GitHub

##Setting up

Fork the GitHub of richelbilderbeek in your own GitHub at the GitHub website.

Clone your fork:

```
git clone https://github.com/your_name/ProjectRichelBilderbeek.git
```

Assign the name 'upstream' to the integration GitHub:

```
cd ProjectRichelBilderbeek
git remote add upstream https://github.com/richelbilderbeek/ProjectRichelBilderbeek.git
```

##Adding changes

```
git add --all :/
git commit -m "your description of the commit here"
git push origin develop
```

If you want the commit to be added to the intergration GitHub, submit a Pull Request from your own GitHub's page

##Updating from the integration GitHub

To pull your own code:

```
git pull origin develop
```

To update with the integration GitHub:

```
git pull upstream develop
```
