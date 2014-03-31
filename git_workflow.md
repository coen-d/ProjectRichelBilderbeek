#git workflow

This GitHub uses a [forking workflow](https://www.atlassian.com/git/workflows#!workflow-forking), where richelbilderbeek h is GitHub is used for intergration of the forks.

The workflow has multiple phases:
- Setting up
- Adding changes
- Updating from the inegration GitHub

##Setting up

Fork the GitHub of richelbilderbeek in your own GitHub at the GitHub website.

Clone your fork:

<code>
git clone https://github.com/your_name/ProjectRichelBilderbeek.git
</code>

Assign the name 'upstream' to the integration GitHub:

<code>
cd ProjectRichelBilderbeek
git remote add upstream https://github.com/richelbilderbeek/ProjectRichelBilderbeek.git
</code>

##Adding changes

<code>
git add .
</code>

<code>
git add --all
</code>

<code>
git commit -m "your description of the commit here"
</code>

<code>
git push origin develop
</code>

If you want the commit to be added to the intergration GitHub, submit a Pull Request from your own GitHub's page

##Updating from the integration GitHub

To pull your own code:

<code>
git pull origin develop
</code>

To update with the integration GitHub:

<code>
git pull upstream develop
</code>
