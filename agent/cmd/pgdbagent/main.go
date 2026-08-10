/***************************************************************************
 *  PGDBATOOLS AGENT
 *
 *  START  : 2026-08-06
 *  AUTHOR : Marco Túlio Castro
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

package main

import (
	"context"
	"flag"
	"fmt"
	"log"
	"pgdbagent/internal/config"
	"pgdbagent/internal/db"
	"pgdbagent/internal/loader"
)

func main() {

	ctx := context.Background()
	configPath := flag.String("config", "config.yaml", "Configuration File")
	flag.Parse()

	cfg, err := config.Load(*configPath)
	if err != nil {
		log.Fatal(err)
	}
	if err := cfg.Validate(); err != nil {
		log.Fatal(err)
	}
	if cfg != nil {
		_ = db.Connect(ctx, cfg)
	}

	metrics, err := loader.Load(cfg)
	if err != nil {
		log.Fatal(err)
	}

	for _, metric := range *metrics {

		fmt.Println("Group: ", metric.Group, " Cluster Version", metric.PostgreSQLVersion)

		for _, query := range metric.Queries {
			fmt.Println(query.Name)
			fmt.Println(query.Query)

		}

	}

}
