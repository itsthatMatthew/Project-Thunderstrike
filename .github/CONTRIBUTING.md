# Project contribution guidelines

We welcome every and anyone wanting to improve our project! If you want to expand the codebase, fix some bugs, or just add new experimental features, feel free to contribute in any way!

Some useful places to get your bearings of:
- Checking out our [README](README.md) should give you a clear overview of the project.
- You can view existing [Issues](https://github.com/itsthatMatthew/Project-Thunderstrike/issues) or create new ones to tackle problems within the codebase.
- You can look up the [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html) to get an idea of the code style we expect.
- Forking and creating [pull requests](https://github.com/itsthatMatthew/Project-Thunderstrike/pulls) are the most immediate way you can help us.

If you have further questions about the GitHub ecosystem, give their [support page](https://support.github.com/) a look.

## Issues

Issues are a clear and direct way for us to focus on the important bits. Whether these are bug reports, feature requests or optimizations, opening an issue is a valuable step in improving the state of the codebase.

### Creating issues

If you find an issue within the project, search the [Issues](https://github.com/itsthatMatthew/Project-Thunderstrike/issues), a relevant one might already exist. If it does not, feel free to open a new one, providing the necessary information.

### Solving issues

For a more hands on experience, you can find issues you want to solve. If you've made the necessary changes, you can create a pull request so that we can review and merge it.

## Conventions

Reading existing code might be helpful to get the general vibe.

- When contributing, you should try to always follow the [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html).
- If some fuzzy edge case is not well covered, be prudent in your writing: try to be as understandable as possible.
- The project uses the C++17 standard. Utilize language features as best as you can.
- Indentations should be done by using 2 spaces.
- Code with should be no more than 80 characters.
- Code blocks should be wrapped in Allman style - each brace in a new line.
- **ONLY** write single line code if it has a well defined and clear responsibility.
- Operators and variables (among other things) should be separated by spaces for clarity.
- Only ever use `using namespace ...` locally, but avoid if possible.

### Naming conventions

Adhering to these practices will make the code immediately easier to read for everyone.

- Files and variable names should be written in snake case (`snake_case`).
- Defines, template variables and enum values should be written in macro case (`MACRO_CASE`).
- Class and type names should be written in uppre camel case (`UppreCamelCase`).
- Functions should be written in lower camel case (`lowerCamelCase`).
- You should **NEVER** start any name with an underscore (`_...`).
- Use prefixes for class variables, such as `m_` (member), `s_` (static), `c_` (const).

## Testing

Testing is done with the Unity framework PlatformIO provides. If you add new functionality, you should create a new unit test for it as well. When adding multiple features, creating a subfolder might be a good idea.

Additional code scanning is done to static analyze the codebase and check for vulnerabilities.

If your code fails the project's default unit tests, CI tests or security scans, it will not be merged.

## Forking and creating branches

You're welcomed to fork our repository! This way you can easily to develop and add new features locally, then if you feel ready, create pull requests to merge your code into our codebase!

For making this process easier for everyone, we recommend you be aware of the following branches and namings:

- `main`: the main branch of the project, containing the stable version (according to best efforts). 
- `devel`: constant development branch, from which new features are pulled into main.
- `exp-`: prefix for new and experimental features, which are still in early development.
- `fix-`: prefix for fixing bugs and solving issues in already written code.
- `upd-`: prefix for updating or expanding already written code.

## Creating pull requests

Pull requests let you improve the codebase in the most hands-on way possible.

- After forking the repository, create a new working branch for your needs.
- You should add your fixes and improvements in smaller chunks: each commit should tackle a single and well-defined problem.
- Your pull requests should have a straight to the point title and through description. If you are solving an issue, link it to the pull request.

If everything is fine, your pull request should be merged.

---

<footer style="text-align: center;">
The provided templates should give you a clear direction of the expected issue and pull request formatting.

If you have further questions, feel free to contact us.
</footer>