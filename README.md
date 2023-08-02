# @uclip/clipboard

NodeJS addon that provides a convenient and easy-to-use interface for working with the system clipboard. It allows you to read and write text and files from/to the clipboard and provides event-based notifications for changes in the clipboard content.

## Installation

```
npm i @uclip/clipboard
```

## Usage

To use the Clipboard Library in your project, you need to require or import it, depending on your project setup.

```js
const { Clipboard } = require('@uclip/clipboard')

const clipboard = new Clipboard({
	interval: 100 // optinal
})

clipboard.on('text-changed', console.log)
```

### Reading text from the clipboard

```js
const clipboard = new Clipboard()

console.log(clipboard.read()) // what ever is in the clipboard at the time
```

### Writing text to the clipboard

```js
const clipboard = new Clipboard()

clipboard.write('Hello from the other side!')

console.log(clipboard.read()) // "Hello from the other side!"
```

### Handling clipboard change events

The `Clipboard` object extends `EventEmitter` an inherits all of its useful members like `on`, `once`, `addListener`, etc.

There are two events available to subscribe to:

- `text-changed` - when text has been copied.
- `file-changed` - when files have been copied.

```js
const clipboard = new Clipboard()

clipboard.on('text-changed', text => {
	console.log('Clipboard text changed:', text)
})

clipboard.on('file-changed', paths => {
	console.log('Clipboard file changed:', paths)
})
```

## API

The `Clipboard` object provides useful methods for interacting with the system clipboard:

### `clipboard.read(): string`

Reads the text from the clipboard and returns it as a string.

### `clipboard.write(text: string): void`

Writes the given text to the clipboard.

### `clipboard.readFiles(): string[]`

Reads the file paths from the clipboard. (Note: Platform-specific support)

### `clipboard.writeFiles(paths: string[]): void`

Writes the given file paths to the clipboard. (Note: Platform-specific support)

### `clipboard.clear(): void`

Clears the clipboard content.

### `clipboard.stop(): void`

Stops the clipboard change event monitoring.

## Contributing

Contributions to the Clipboard Library are welcome! If you find any issues or have suggestions for improvement, please feel free to open an issue or submit a pull request.

## License

The Clipboard Library is open-source and distributed under the MIT License. See the LICENSE file for more details.
