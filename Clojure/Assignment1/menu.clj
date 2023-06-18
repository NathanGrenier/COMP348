(ns menu)

(defn getCommand 
  []
  (print "Enter an option: ")
  (flush)
  (read-line)
  )

(defn displayMenu
  []
  (println 
  "
   *** SIS Menu ***
  ------------------
  1. Display Courses
  2. Display Students
  3. Display Grades
  4. Display Student Record
  5. Calculate GPA
  6. Course Average
  7. Exit
  "
   ))