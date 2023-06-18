(ns app
  (:require [db])
  (:require [menu])
  (:require [clojure.pprint :refer [pprint]])
  )

(def cmd (atom -1))

(def courseDB (db/loadData "courses.txt"))
(def studDB (db/loadData "studs.txt"))
(def gradeDB (db/loadData "grades.txt"))

(defn start []
  (menu/displayMenu)
  (reset! cmd (menu/getCommand))
  (if (not= @cmd "7") 
    (do
      (case @cmd
        "1" (dorun (map pprint courseDB))
        "2" (dorun (map pprint studDB))
        "3" (dorun (map pprint gradeDB))
        "4" (db/displayStudentRecords studDB gradeDB courseDB)
        "5" (db/calGPA gradeDB courseDB)
        "6" (db/courseAVG gradeDB courseDB)
        (println "Not a valid command"))
      (recur))
    (do
      (println "Exiting Program."))
    )
  )

(start)