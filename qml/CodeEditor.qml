import QtQuick 2.1
import QtQuick.Controls 1.0
import Highlighters 1.0
Rectangle {
	id: codeEditor
	property alias textDocument: myArea.textDocument
	property alias flickableItem: myArea.flickableItem
	
	gradient: Gradient {
		GradientStop { position: 0.0; color: "lightsteelblue" }
		GradientStop { position: 1.0; color: "lightblue" }
	}
	
	Rectangle {
		color: "white"
		anchors.left: parent.left
		anchors.right: parent.right
		y: myArea.positionToRectangle(myArea.cursorPosition).y + 6 
		height: myArea.positionToRectangle(myArea.cursorPosition).height
		
		visible: false
	}
	
	Column {
		anchors.fill: parent
		anchors.topMargin: -flickableItem.contentY + 6
		Repeater {
			model: myArea.lineCount
			Rectangle {
				color: index % 2 == 0 ? "lightblue" : "lightgray"
				anchors.left: parent.left
				anchors.right: parent.right
				height: myArea.positionToRectangle(0).height
			}
		}
	}
	
	TextArea {
		id: myArea
		property PythonHighlighter codeHighlighter: PythonHighlighter {}
		
		backgroundVisible: false
		anchors.fill: parent
		text: "# You can press Ctrl-R to run the program\nprint(\"Hello World!\")"
		
		//font.pointSize: 18
	
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