# Weekly releases

The [Weekly release workflow](../.github/workflows/release.yml) runs every Monday
at 06:17 UTC on `main`. It also supports **Actions → Weekly release → Run workflow**
with `main` selected. GitHub may delay scheduled runs.

Each run:

1. Runs the existing CI workflow against the source commit.
2. Runs `python scripts/update_version.py --bump-patch`. For example,
   `0.0.4-unstable` becomes `0.0.5-unstable`; major, minor, and release status stay
   unchanged. Both `VERSION.txt` and Doxygen's `PROJECT_NUMBER` are updated.
3. Builds Doxygen HTML with Graphviz from the versioned release source.
4. Pushes the version commit to `main` and its `vX.Y.Z-status` tag atomically.
5. Creates a GitHub release with generated release notes. Versions ending in
   `-unstable` are marked as prereleases. GitHub provides source archives;
   this workflow does not build binary packages.
6. Publishes the generated HTML to this repository's GitHub Pages site, replacing
   the previous API documentation with documentation for the release.

A patch release is created each week even when there are no other source changes.
Documentation deployment is part of this workflow because pushes and releases
made with `GITHUB_TOKEN` do not start additional workflows.

## Repository setup

- Merge the workflow changes into the default branch, `main`.
- Under **Settings → Pages → Build and deployment**, set **Source** to
  **GitHub Actions**. The expected project URL is
  <https://nexilislib.github.io/nexilis/> unless a custom domain is configured.
  This deploys the API documentation as the entire Pages site.
- Allow GitHub Actions to write repository contents and create tags/releases.
  Branch and tag rules must allow the workflow's version commit and release tag;
  this workflow does not bypass protection rules. If required pull requests prevent
  bot pushes, adjust the repository's release policy before enabling the schedule.
- The `github-pages` environment must allow deployments from `main`. Required
  environment reviewers will make publication wait for approval.

The workflow uses the repository's `GITHUB_TOKEN`; no personal token is needed
when repository rules permit these operations. See GitHub's
[custom Pages workflow documentation](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).

## Failures and retries

CI, Pages configuration, or documentation build failures prevent the release push.
If `main` changes during the run, the push fails without overwriting those changes;
start a new manual run against the updated `main`.

If a run fails after the push, rerun its failed jobs to finish the same release.
The workflow recognizes its existing tag and does not increment the version again.
Starting a new manual run creates a new patch release. The Pages job checks that
`main` still has the release version and refuses to publish a superseded release.

For local version changes, install the script dependencies with
`python -m pip install ./scripts`, then run either:

```sh
python scripts/update_version.py --bump-patch
python scripts/update_version.py 0.1.0-unstable
```
