import QtQuick 2.1
import QtQuick.Controls 1.0
import Highlighters 1.0
Rectangle {
	
	property alias textDocument: myArea.textDocument
	
	gradient: Gradient {
		GradientStop { position: 0.0; color: "lightsteelblue" }
		GradientStop { position: 1.0; color: "lightblue" }
	}
	
	TextArea {
		id: myArea
		property PythonHighlighter codeHighlighter: PythonHighlighter {}
		
		backgroundVisible: false
		anchors.fill: parent
	
		function tryIt() {
			if (textDocument)
			{
				codeHighlighter.setDocument(textDocument)
				console.log("Highlighter installed")
			}
		}
	
		Component.onCompleted: tryIt()
	}
}