.import QtQuick.LocalStorage 2.0 as Sql
function createDatabase(db) {
	db.transaction(function(tx) {
		tx.executeSql('CREATE TABLE IF NOT EXISTS settingsset (mkey TEXT PRIMARY KEY, value)')
	})
}

function openDatabase() {
	var myDb = Sql.LocalStorage.openDatabaseSync("DeliciousSettings", "", "Settings of delicious pie", 100500, createDatabase)
	return myDb
}

function getSettingValue(db, key) {
	var result = undefined
	db.transaction(function(tx) {
		var rs = tx.executeSql('SELECT * FROM settingsset WHERE mkey = ?', key)
		if (rs.rows.length > 0)
			result = rs.rows.item(0).value
	})
	return result
}

function setSettingValue(db, key, value) {
	db.transaction(function(tx) {
		tx.executeSql('INSERT OR REPLACE INTO settingsset VALUES (?, ?)', [key, value])
	})
}