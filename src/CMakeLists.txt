target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
    scheduler.cpp
    job_gen.cpp
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    FILE1="${PROJECT_SOURCE_DIR}/job5.txt"
    FILE2="${PROJECT_SOURCE_DIR}/job10.txt"
    FILE3="${PROJECT_SOURCE_DIR}/job15.txt"
)

target_sources(job_gen
    PRIVATE
    job_gen_main.cpp
    job_gen.cpp
)
