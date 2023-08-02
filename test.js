const { Clipboard } = require('./index')

describe('native functions', () => {
	let clipboard

	const someText = 'some text'
	const someOtherText = 'some other text'
	const someFiles = ['/path/to/file1', '/path/to/file2']
	const someOtherFiles = ['/path/to/file1', '/path/to/file2', '/path/to/file3']

	beforeEach(() => {
		clipboard = new Clipboard()
		clipboard.clear()
	})

	afterEach(() => {
		clipboard.stop() // otherwise tests hang because of the watcher
	})

	test('read() should return the text from the clipboard', () => {
		clipboard.write(someText)
		expect(clipboard.read()).toBe(someText)
	})

	test('write() should write the given text to the clipboard', () => {
		clipboard.write(someText)
		expect(clipboard.read()).toBe(someText)
		clipboard.write(someOtherText)
		expect(clipboard.read()).toBe(someOtherText)
	})

	test('readFiles() should return an array of file paths from the clipboard', () => {
		expect(clipboard.readFiles()).toEqual([])
		clipboard.writeFiles(someFiles)
		expect(clipboard.readFiles()).toEqual(someFiles)
	})

	test('writeFiles() should write the given file paths to the clipboard', () => {
		expect(clipboard.readFiles()).toEqual([])
		clipboard.writeFiles(someFiles)
		expect(clipboard.readFiles()).toEqual(someFiles)
		clipboard.writeFiles(someOtherFiles)
		expect(clipboard.readFiles()).toEqual(someOtherFiles)
	})

	test('clear() should clear the clipboard content', () => {
		clipboard.write(someText)
		clipboard.clear()
		expect(clipboard.read()).toBe('')
		clipboard.writeFiles(someFiles)
		clipboard.clear()
		expect(clipboard.readFiles()).toEqual([])
	})
})
