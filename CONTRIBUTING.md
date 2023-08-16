# Project contribution guidelines

We welcome every and anyone wanting to improve our project! If you want to expand the codebase, fix some bugs or just add new experimental features, feel free to contribute in any way!

Some useful places to get your bearings of:
- Checking out our [README](README.md) should give you a clear overview of the project.
- You can view existing [Issues](https://github.com/itsthatMatthew/Project-Thunderstrike/issues) or create new ones to tackle problems within the codebase.
- You can look up the [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html) to get an idea of the codestyle we expect.
- Forking and creating [pull requests](https://github.com/itsthatMatthew/Project-Thunderstrike/pulls) are the most immediate way you can help us.

If you have further questions about the GitHub ecosystem, give their [support page](https://support.github.com/) a look.

## Issues

Issues are a clear and direct way for us to focus on the important bits. Whether these are bug reports, features requests or optimizations, opening an issue is a valuable step in improving the state of the codebase.

### Creating issues

If you fing an issue within the project, search the [Issues](https://github.com/itsthatMatthew/Project-Thunderstrike/issues), a relevant one might already exist. If it does not, feel free to open a new one, providing the necessary information.

### Solving issues

For a more hands on experience, you can find issues you want to solve. If you've made the necessary changes, you can create a pull request so that we can review and merge it.

## Conventions

Reading existing code might be helpful to get the general vibe.

- When contributing, you should always follow the [LLVM Conding Standards](https://llvm.org/docs/CodingStandards.html).
- If some fuzzy edgecase is not well covered, be prudent in your writing: try to be as understandable as possible.
- The project uses the C++17 standard. Utilize language features as best as you can.
- Indentations should be done by using 2 spaces.
- Namings should be snake_case everywhere.

## Testing

Testing is done with the Unity framework PlatformIO provides. If you add new functionality, you should create a new unit test for it as well. When adding multiple features, creating a subfolder might be a good idea.

If your code fails the project's default unit and CI tests, it will not be merged.

## Creating pull requests

Pull requests let you improve the codebase in the most hands on way possible.

- After forking the repository, create a new working branch for your needs.
- You should add your fixes and improvements in smaller chunks: each commit should tackle a single and well defined problem.
- Your pull requests should have a straight to point title and through description. If you are solving an issue, link it to the pull request.

If everything is fine, your pull request should be merged.