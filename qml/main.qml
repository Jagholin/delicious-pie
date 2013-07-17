import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
	id: app
	visible: true
	
	menuBar: MenuBar {
		Menu {
			title: "File"
			MenuItem {
				text: "Save"
				shortcut: "Ctrl+S"
				onTriggered: app.saveDoc()
			}
			MenuItem {
				text: "run"
				shortcut: "Ctrl+R"
				onTriggered: mainApp.runDocument(centralEditor.textDocument)
			}
			MenuItem {
				text: "Quit"
				shortcut: "Ctrl+Q"
				onTriggered: Qt.quit()
			}
		}
		Menu {
			title: "Help"
			MenuItem {
				text: "About..."
				shortcut: "Ctrl+H"
				onTriggered: app.about()
			}
		}
	}
	
	CodeEditor {
		id: centralEditor
		anchors.fill: parent	
	}
	
	Rectangle {
		id: aboutView
		visible: false
		z: 1
		
		anchors.fill: parent
		color: "green"
		state: "off"
		
		MouseArea {
			anchors.fill: aboutView
			onClicked: aboutView.state = "off"
			cursorShape: Qt.ArrowCursor
		}
		Rectangle {
			id: innerAboutRect
			
			anchors.centerIn: parent
			width: 250
			height: 100
			color: "white"
			border.color: "black"
			opacity: 1
			Text {
				anchors.fill: parent
				anchors.margins: 4
				text: "Simple Python IDE for beginners <br> by Jagh, 2013"
			}
		}
		
		states: [ State {
			name: "off"
			PropertyChanges { target: aboutView; opacity: 0 }
			PropertyChanges { target: aboutView; enabled: false }
		}, State {
			name: "on"
			PropertyChanges { target: aboutView; opacity: 0.5 }
			PropertyChanges { target: aboutView; enabled: true } 
			PropertyChanges { target: centralEditor; enabled: false }
		} ]
		transitions: Transition {
			from: "off"; to: "on"
			NumberAnimation { properties: "opacity"; duration: 500 }
		}
	}
	
	FileDialog {
		id: fileChooser
		title: "Choose where to save the file"
		nameFilters: "*.py"
		selectExisting: false
		selectFolder: false
		selectMultiple: false
		
		onAccepted: mainApp.saveDocument(centralEditor.textDocument, fileUrl)
	}
	
	function about() {
		aboutView.visible = true
		aboutView.state = "on"
	}
	
	function saveDoc() {
		fileChooser.open()
	}
}
