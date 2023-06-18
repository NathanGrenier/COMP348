(ns db
  (:require [clojure.string :as str])
  (:require [clojure.pprint :refer [pprint]])
  (:require [clojure.java.io]))

(def letterGrades {
"A+" 4.3, "A" 4, "A-" 3.7,
"B+" 3.3, "B" 3, "B-" 2.7,
"C+" 2.3, "C" 2, "C-" 1.7,
"D+" 1.3, "D" 1, "D-" 0.7,
"F" 0 }
)

(defn- collToMap
  [coll]
  (reduce (fn [acc [k & v]] (assoc acc k v)) {} coll)
  )

(defn loadData 
  [filename]
  (if (.exists (clojure.java.io/file filename))
    (mapv #(str/split % #"\|") (str/split-lines (slurp filename)))
    (do
      (println (str "ERROR: File <" filename "> could not be found.\nNOTE: Files in this assignment are hardcoded. You should define these files in the same directory as the app.clj file: [studs.txt, grades.txt, courses.txt]"))
      (System/exit 0)) 
    ) 
  )

; Find out how to sort by course name and number
(defn displayStudentRecords
  [studDB gradeDB courseDB]
  (let [id (do
             (print "Enter an id: ")
             (flush)
             (read-line))
        student (into [] (take 2 (nth (filter #(= (nth % 0) id) studDB) 0)))
        grades (filter #(= (nth % 0) id) gradeDB)]
    (pprint student)
    (doseq [grade grades]
      (let [courseID (nth grade 1)
            course (nth (filter #(= (nth % 0) courseID) courseDB) 0)
            details (into [] (concat (map #(nth course %) [1 2 4]) (map #(nth grade %) [2 3])))]
        (pprint details)
        )
      )
    )
  )

(defn- weightedAVG 
  [grades weights]
  
  (/ (reduce + (map * grades weights)) (reduce + weights))
  )

(defn calGPA
  [gradeDB courseDB]
  (let [studentID (do
                    (print "Enter an id: ")
                    (flush)
                    (read-line))
        grades (map #(get letterGrades %) (map #(nth % 3) (filter #(= (nth % 0) studentID) gradeDB))) 
        courseMap (collToMap courseDB) 
        courseWeights (map #(read-string (nth % 2)) (map #(get courseMap %) (map #(nth % 1) (filter #(= (nth % 0) studentID) gradeDB))))]
        ;courseMap (map #(zipmap [:courseID :courseName :courseNumber :weight :desc] %) courseDB)
    (pprint (str "GPA=" (weightedAVG grades courseWeights)))
    )
  )

(defn- avg 
  [coll]
  (/ (reduce + coll) (count coll))
  )

; I don't know what to do with the course semester. They're all the same
(defn courseAVG
  [gradeDB courseDB]
  (doseq [course courseDB]
    (let [courseID (nth course 0)
          course (nth (filter #(= (nth % 0) courseID) courseDB) 0)
          grades (map #(get letterGrades %) (map #(nth % 3) (filter #(= (nth % 1) courseID) gradeDB)))]
      (pprint (into [] (concat (map #(nth course %) [1 2]) (vector (avg grades)))))
      )
  )
)
