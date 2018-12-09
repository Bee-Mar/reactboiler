# reactboiler
Because React is fun and boiler-plate code isn't.

Let's be real, boiler-plate code sucks. That's why I created ***reactboiler***. It's very simple command line utitlity that will speed up React development a bit, and save you some keystrokes. 

**Command line options supported:**

`--dir` will create a new directory and store all the new file inside.

`--styled` will create a template <a href="https://github.com/styled-components/styled-components">styled-component</a> as a `div`

`--class` will create a template React class

`--redux` will create several template files for Redux actions, action types, 

`--jsx` will create a React or styled-component with a `.jsx` extension. If used with both `--class` and `--styled`, the class component will be given the `.jsx` extension

**Warning:** If you do dare to look under the hood, and view the source code, it is far from my best work. I threw this together in about an hour and a half, and it worked, so I didn't bother changin anything. Feel free to make suggestions to improve and/or extend this CLI.

**Installation:**
The first target is the ***install*** target, so a simple "make" can be executed.

**Usage:** View the man page and screenshots below for more details and examples

This CLI can be used to create up to 128 new directories with React components at once.