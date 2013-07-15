import QtQuick 2.1
import QtQuick.Controls 1.0
import Highlighters 1.0

TextArea {
	id: myArea
	property PythonHighlighter codeHighlighter: PythonHighlighter {}
	
	function tryIt() {
		console.log("Loading completed")
		if (textDocument)
		{
			codeHighlighter.setDocument(textDocument)
			console.log("Highlighter installed")
			return false
		}
		return true
	}
	
	Component.onCompleted: tryIt()
}